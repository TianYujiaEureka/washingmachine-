
#include <string.h>

#include "Data_.h"
#include "USB_To_Usart.h"	

#include "oled.h"
DataFrame dataFrame={0};
uint64_t DeviceID;
uint32_t  UserPwd;
//校验和计算方法：
//offset 便宜量
//frameLen 总长度
unsigned char GetSumCheckNum(char * buf, int offset, int frameLen)
{
	int i = offset;
 unsigned char xorResult = 0;
	
 for (; i < frameLen; i++)
 {
  xorResult ^= buf[i];
 }
 return xorResult;
}
//此方法是将数据做高大小端处理（比如0x1234处理后为0x3412）
//*buf 拼接数据的数组指针 
//data要转化的数据值 ,最大为：0xFFFFFFFF
//len 要转化的字节数,最大为：4
//*count 数组下标
void bigToSmall(unsigned char * buf,uint32_t data,int len,int *count){
	int i=0;
	int l=*count;
	
	for(;i<len;i++){
		buf[l++]=data>>8*(i)&0xFF;
	}
	*count=l;
}

//拼接 上传的数据包
//TxBuffer 发送数据的数组 len忽略，sendData[]上传的数据，dataLen上传数据个数（不得大于32）
int getData(unsigned char *TxBuffer,int *len,uint32_t sendData[],int dataLen){
	int i=0;
	int j=0;
	int count=0;
	int ChecksumCursor=0;
	//拼接帧头
	TxBuffer[i++]=0x7E;
	//拼接帧类型
	TxBuffer[i++]=0x01;
	//拼接设备ID
	 count=DEVICE_ID_LEN;
	while(count--){
		TxBuffer[i++]=DeviceID>>(8*count)&0xFF;
	}
	//记录效验和位置
	ChecksumCursor=i++;//数据区目前没有数值，等后面添加了数据区在给效验和赋值,
	//密码拼接
	 count=PWD_COUNT_LEN;
	while(count--){
		TxBuffer[i++]=UserPwd>>(8*count)&0xFF;
	}
	
	//拼接数据长度（数据区+帧尾）
	TxBuffer[i++]=1+dataLen*(DATA_LEN+1)+1;
	//拼接数据组个数
	TxBuffer[i++]=dataLen;
	
	for(;j<dataLen;j++){
		TxBuffer[i++]=j+1;//数据序号
		//具体数据，每组数据4个字节，所以要将数据拆开
		bigToSmall(TxBuffer,sendData[j],DATA_LEN,&i);
	}

	//数据区已经设置数据我们来完成效验和的赋值
	TxBuffer[ChecksumCursor]=GetSumCheckNum((char *)TxBuffer,DATA_FRAME_HEAD_LEN,i);
	/**/
	//拼接帧尾
	TxBuffer[i++]=0x7E;
	
	return i;
}

void initDataFrame(uint64_t  DeviceID_,uint32_t  UserPwd_){
	DeviceID=DeviceID_;
	UserPwd=UserPwd_;
}


//解析服务器数据（拆包）
int ParseServerData( u8 * src, u8 len){
	
	char mySumCheck;
	int j=0;
	int k=0;
	int i=0;
	int count=0;
	//判断是否符合最小数据长度
	if(len<22){
		SendDataToDevice_a("服务器数据长度不达标\r\n",22);
		return 0;
	}
	
	if((dataFrame.FrameHead=src[i++])!=0x7E){
		SendDataToDevice_a("没有找到数据头\r\n",16);
		return 0;
	}
	//数据类型
	if((dataFrame.FrameType=src[i++])!=0x01){
		SendDataToDevice_a("数据头类型异常\r\n",16);
		return 0;
	}

	count=DEVICE_ID_LEN;
		dataFrame.DeviceID=0;//数值清零
	while(count--){
		dataFrame.DeviceID+=(uint64_t)src[i++]<<(8*count);
		
	}
	//比对设备ID
	if(dataFrame.DeviceID!=DeviceID){
		SendDataToDevice_a("设备ID异常\r\n",12);
		return 0;
	}
	//效验和
	dataFrame.Checksum=src[i++];
	//计算效验和，不包含帧尾所以（len-1）
	mySumCheck=GetSumCheckNum((char*)src,DATA_FRAME_HEAD_LEN,len-1);
	//比对效验和
	if(dataFrame.Checksum!=mySumCheck){
		SendDataToDevice_a("效验和异常\r\n",12);
		return 0;
	}
	//密码
	count=PWD_COUNT_LEN;
	dataFrame.UserPwd=0;//数值清零
	while(count--){
		dataFrame.UserPwd+=(uint32_t)src[i++]<<(8*count);
		
	}
	//比对设备密码（服务器的指令数据密码为空闲，所以不做密码比较）
	//if(Pwd!=UserPwd){
		//SendDataToDevice_a("设备密码异常\r\n",14);
	//	return 0;
	//}
	
	
	//数据指令长度
	dataFrame.DataLen=src[i++];
	if(dataFrame.DataLen!=(len-i)){
		SendDataToDevice_a("数据长度异常\r\n",14);
		return 0;
	}
	dataFrame.mOrderDataCount=src[i++];
	
	for(;k<dataFrame.mOrderDataCount;k++){
		i++;//数据编号
		dataFrame.mOrderConnect[k]=0;//数值清零
		for(;j<4;j++){
			dataFrame.mOrderConnect[k]+=src[i++]<<8*j;
		}
	
	}
	//结尾
	if((dataFrame.FrameTail=src[i++])!=0x7E){
		SendDataToDevice_a("没有找到数据尾\r\n",16);
		return 0;
	}
	return i;
}


