/********************************************************************
*Copyright(C）2014-2016,沈阳艾克申机器人技术开发有限责任公司
*FileName：	   ringBuffer.c
*Author：      Peng Xu
*Date：        2016/10/28
*Description： 环形数组的建立
*		       
*
*Version：     V1.0
*
********************************************************************/

#include "ringbuffer.h"
#include "MotionCard.h"
#include <stdlib.h>



//环形缓冲区中的元素总数量  注意这些值在控制卡长期使用时可能会溢出！！！！
static int      upPointer = 0;
static int      g_iput = 0;
static int      downPointer = 0;
static float    lengthSum = 0.0f;
static int      countMax = 0;



//缓存区
KeyPointInf_t *ringBuffer;
	
	
	
/*********************************************************************************
* @name 	BufferZizeInit
* @brief	缓存池初始化函数
* @param	num 最大存取点个数，一个点占28字节;
* @retval 无
**********************************************************************************/
void BufferZizeInit(int num)
{
	countMax = num;
	ringBuffer = (KeyPointInf_t*)malloc(sizeof(KeyPointInf_t) * countMax);
}
	
/*********************************************************************
* @name 		addring
* @brief  	环形缓冲区的地址编号计算函数，如果到达唤醒缓冲区的尾部，将绕回到头部。
						环形缓冲区的有效地址编号为：0到(COUNTMAX-1)
* @param  	i：传入上指针值
* @retval 	对应数组的编号
********************************************************************/
static int addring(int i)
{
	return (i + 1) == countMax ? 0 : i + 1;
}



/*********************************************************************
* @name 		addring
* @brief  	向环形缓冲区中放入一个点
* @param  	pose：添加的元素
* @retval 	返回是否将环形数组填满 1：未满； 0：已经满了
********************************************************************/
int PutRingBuffer(KeyPointInf_t pose)
{
	if ((upPointer - downPointer) < countMax - 2)
	{
		ringBuffer[g_iput] = pose;
		g_iput = addring(g_iput);
		upPointer++;
		return 1;
	}
	//buffer已满
	else
	{
		return 0;
	}
}



/*********************************************************************
* @name 		GetRingBufferPoint
* @brief  	返回一个点    
* @param  	num取1代表取区间里的第一个数，没有第0个数
* @retval 	返回所取点坐标
********************************************************************/
Point_t GetRingBufferPoint(int num)
{
	int realNum;
	realNum = num - 1 + downPointer;
	return ringBuffer[realNum % countMax].point;
}


//设置速度
void SetRingBufferPointVell(int num,float vell)
{
	int realNum;
	realNum = num - 1 + downPointer;
	ringBuffer[realNum % countMax].vellMax = vell;
}


//返回速度
float GetRingBufferPointVell(int num)
{
	int realNum;
	realNum = num - 1 + downPointer;
	return ringBuffer[realNum % countMax].vellMax;
}


/*********************************************************************
* @name 		GetRingBufferPointAngle
* @brief  	返回一个角度    
* @param  	num取1代表取区间里的第一个数，没有第0个数
* @retval 	返回所取点角度信息
********************************************************************/
float GetRingBufferPointAngle(int num)
{
	int realNum;
	realNum = num - 1 + downPointer;
	return ringBuffer[realNum % countMax].angle;
}


/*********************************************************************
* @name 		GetRingBufferPointPoseAngle
* @brief  	返回一个三轮的姿态角度    
* @param  	num取1代表取区间里的第一个数，没有第0个数
* @retval 	返回所取点角度信息
********************************************************************/
float GetRingBufferPointPoseAngle(int num)
{
	int realNum;
	realNum = num - 1 + downPointer;
	return ringBuffer[realNum % countMax].poseAngle;
}

//返回改点据起点的路径长度
float GetRingBufferPointLen(int num)
{
	int realNum;
	realNum = num - 1 + downPointer;
	return ringBuffer[realNum % countMax].length;
}


//返回该段的曲率半径平均值
float GetRingBufferAverCurvature(int num)
{
	int realNum;
	realNum = num - 1 + downPointer;
	return ringBuffer[realNum % countMax].curvatureR;
}



/*********************************************************************
* @name 		GetCount
* @brief  	返回储存器元素个数    
* @param  	无
* @retval 	无
********************************************************************/
int GetCount(void)
{
	return (upPointer - downPointer);
}




/*********************************************************************
* @name 		GetUpPointer
* @brief  	返回上指针   
* @param  	无
* @retval 	无
********************************************************************/
int GetUpPointer(void)
{
	return upPointer;
}


/*********************************************************************
* @name 		GetDownPointer
* @brief  	返回下指针   
* @param  	无
* @retval 	无
********************************************************************/
int GetDownPointer(void)
{
	return downPointer;
}


/*********************************************************************
* @name 		DeleteData
* @brief  	删除数据点  
* @param  	num 所要删除点的个数。从第一个点开始删除
* @retval 	无
********************************************************************/
void DeleteData(int num)
{
	downPointer = num + downPointer;
}



/*********************************************************************
* @name 		SetUpPointer
* @brief  	设置上指针   
* @param  	无
* @retval 	无
********************************************************************/
void SetUpPointer(int num)
{
	upPointer = num;
} 

//获取缓存池的首地址
u32* GetFristAdress(void)
{
	return (u32 *)ringBuffer;
}


//获取终点据起点的路径长度
float GetLength(void)
{
	return lengthSum;
}


//设置终点据起点的路径长度
void SetLength(float len)
{
	lengthSum = len;
}


//清空ringBuffer
void ClearRingBuffer(void)
{
	upPointer = 0;
	g_iput = 0;
	downPointer = 0;
	lengthSum = 0.0f;
}
