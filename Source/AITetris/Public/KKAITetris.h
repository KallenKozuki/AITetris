// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KKAITetris.generated.h"

UCLASS()
class AITETRIS_API AKKAITetris : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKKAITetris();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//基础棋盘
	int Map[20][10] =
	{
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},

		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},

		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},

		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0}
	};

	//所有俄罗斯方块类型
	int Types[7][4] =
	{
		1,3,5,7,	//I
		2,4,5,7,	//Z
		3,5,4,6,	//S
		3,5,4,7,	//T
		2,3,5,7,	//J
		3,5,7,6,	//L
		2,3,4,5		//O
	};

	UPROPERTY(BlueprintReadOnly)
	int Score = 0;//得分

	int CurrentTetromino[4][2];//当前方块位置
	int CurrentType;//当前类型
	int CurrentVector = 0;//当前方块的矢量（用于控制旋转）
	enum AllStatus { Falling, Bottom, NotCreate };//状态，枚举类型
	AllStatus Status = NotCreate;
	const int CoolDown = 15.0f;//内置CD
	int GameTime = 10000.0f;
	int TimeLeft = CoolDown;

	//AI 棋盘配置
	int TempMap[20][10];
	int FinalMap[20][10];
	int TempTetromino[4][2];

	//方块生成
	void randomOne();
	void showTetromino();
	void showMap();
	void destroyBlock();

	//方块移动和旋转
	void moveDown();
	void moveRight();
	void moveLeft();
	void rotate();

	//消除行
	void clearFullLine();
	void updateMap(int row);

	//AI 操作
	void resetMap();
	void setTempMap();
	void setFinalMap();
	void setMap();
	void setAICopyMap();
	void AIOperation();

	//AI 计算最佳落点
	void calLHandRC(int T[4][2]);
	void AICalculation(int T[4][2]);
	void setTempTetromino();
	void resetCurrentTetromino();

	//AI 深度优先搜索，本质就是遍历所有的落点位置，寻找最佳落点
	void dfs(int T[4][2]);
	bool checkCollision(int T[4][2]);
	void fall(int Prev[4][2]);


	/*-------------------------------*/
	//PierreDellacheris算法的核心参数
	int LandingHeight;
	int RowCleared;
	int RowTransitions;
	int ColTransitions;
	int HoleSums;
	int WellSums;
	float MaxResult;

	//核心参数的计算函数
	float PierreDellacherieFormule();
	void calRT();
	void calCT();
	void calHS();
	void calWS();
	int calDec(int num);
	int CopyMap[22][12];
};
