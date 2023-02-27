// Fill out your copyright notice in the Description page of Project Settings.


#include "KKAITetris.h"
#include "KKBlock.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h" //迭代器头文件

// Sets default values
AKKAITetris::AKKAITetris()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKKAITetris::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKKAITetris::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GameTime -= DeltaTime;
	/*if (GameTime <= 0)
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Result"));
	}*/
	//根据不同的状态，选择：生成方块，方块下落，更新棋盘
	switch (Status)
	{
	case NotCreate:
		randomOne();
		destroyBlock();
		showTetromino();
		showMap();
		Status = Falling;
		break;
	case Falling:
		TimeLeft -= DeltaTime;
		if (TimeLeft <= 0)
		{
			AIOperation();
			setMap();
			destroyBlock();
			showMap();
			Status = NotCreate;
			TimeLeft = CoolDown;
		}
		break;
	default:
		break;
	}
}

void AKKAITetris::showMap()
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (this->Map[i][j] == 1)
			{
				float X = 0.0 + 10.0 * j;
				float Z = 190.0 - i * 10.0;
				FVector Location(X, 0.0, Z);
				FRotator Rotation(0.0, 0.0, 0.0);
				GetWorld()->SpawnActor<AKKBlock>(Location, Rotation);
				Map[i][j] = 2;
			}
		}
	}
}

void AKKAITetris::showTetromino()
{
	for (int i = 0; i < 4; i++)
	{
		float X = 0.0 + 10.0 * CurrentTetromino[i][0];
		float Z = 190.0 - CurrentTetromino[i][1] * 10.0;
		FVector Location(X, 0.0, Z);
		FRotator Rotation(0.0, 0.0, 0.0);
		GetWorld()->SpawnActor<AKKBlock>(Location, Rotation);
	}
}

void AKKAITetris::randomOne()
{
	CurrentVector = 0;
	CurrentType = FMath::RandRange(0, 6);
	if (CurrentType == 1 || CurrentType == 2)
	{
		int pro = FMath::RandRange(0, 2);
		if (pro == 1 || pro == 2)
		{
			CurrentType = FMath::RandRange(4, 5);
		}
	}
	for (int i = 0; i < 4; i++)
	{
		int BasicBlock = Types[CurrentType][i];
		CurrentTetromino[i][0] = BasicBlock / 2;	//col
		CurrentTetromino[i][1] = BasicBlock % 2;	//row
	}
}

void AKKAITetris::destroyBlock()
{
	for (TActorIterator<AKKBlock> it(GetWorld()); it; ++it)
	{
		it->Destroy();
	}
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (Map[i][j] != 0)
			{
				Map[i][j] = 1;
			}
		}
	}
}

void AKKAITetris::moveDown()
{
	int Temp[4][2];
	for (int i = 0; i < 4; i++)
	{
		Temp[i][0] = CurrentTetromino[i][0];
		Temp[i][1] = CurrentTetromino[i][1];
	}

	//row++
	for (int i = 0; i < 4; i++)
	{
		CurrentTetromino[i][1]++;
		int col = CurrentTetromino[i][0];
		int row = CurrentTetromino[i][1];
		if (row > 19 || Map[row][col] != 0)
		{
			for (int j = 0; j < 4; j++)
			{
				CurrentTetromino[j][0] = Temp[j][0];
				CurrentTetromino[j][1] = Temp[j][1];
			}
			for (int j = 0; j < 4; j++)
			{
				int c = CurrentTetromino[j][0];
				int r = CurrentTetromino[j][1];
				Map[r][c] = 1;
			}
			break;
		}
	}
}

void AKKAITetris::moveRight()
{
	int Temp[4][2];
	for (int i = 0; i < 4; i++)
	{
		Temp[i][0] = CurrentTetromino[i][0];
		Temp[i][1] = CurrentTetromino[i][1];
	}

	for (int i = 0; i < 4; i++)
	{
		CurrentTetromino[i][0]++;
		int col = CurrentTetromino[i][0];
		int row = CurrentTetromino[i][1];
		if (col > 9 || Map[row][col] != 0)
		{
			for (int j = 0; j < 4; j++)
			{
				CurrentTetromino[j][0] = Temp[j][0];
				CurrentTetromino[j][1] = Temp[j][1];
			}
			break;
		}
	}
}

void AKKAITetris::moveLeft()
{
	int Temp[4][2];
	for (int i = 0; i < 4; i++)
	{
		Temp[i][0] = CurrentTetromino[i][0];
		Temp[i][1] = CurrentTetromino[i][1];
	}

	for (int i = 0; i < 4; i++)
	{
		CurrentTetromino[i][0]--;
		int col = CurrentTetromino[i][0];
		int row = CurrentTetromino[i][1];
		if (col < 0 || Map[row][col] != 0)
		{
			for (int j = 0; j < 4; j++)
			{
				CurrentTetromino[j][0] = Temp[j][0];
				CurrentTetromino[j][1] = Temp[j][1];
			}
			break;
		}
	}
}

void AKKAITetris::clearFullLine()
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (Map[i][j] == 0)
			{
				break;
			}
			if (j == 9)
			{
				Score++;
				updateMap(i);
			}
		}
	}
}

void AKKAITetris::updateMap(int row)
{
	for (int i = row; i > 0; i--)
	{
		for (int j = 0; j < 10; j++)
		{
			Map[i][j] = Map[i - 1][j];
		}
	}
}

void AKKAITetris::rotate()
{
	switch (CurrentType)
	{
	case 0:
		//I
		for (int i = 0; i < 4; i++)
		{
			Swap(CurrentTetromino[i][0], CurrentTetromino[i][1]);
			CurrentVector = !CurrentVector;
		}
		break;
	case 1:
		//Z
		if (CurrentVector == 0)
		{
			CurrentVector = 1;
			CurrentTetromino[0][0] = 3;
			CurrentTetromino[1][1] = 2;
		}
		else
		{
			CurrentVector = 0;
			CurrentTetromino[0][0] = 1;
			CurrentTetromino[1][1] = 0;
		}
		break;
	case 2:
		//S
		if (CurrentVector == 0)
		{
			CurrentVector = 1;
			CurrentTetromino[0][0] = 3;
			CurrentTetromino[3][1] = 2;
		}
		else
		{
			CurrentVector = 0;
			CurrentTetromino[0][0] = 1;
			CurrentTetromino[3][1] = 0;
		}
		break;
	case 3:
		//T
		if (CurrentVector == 0)
		{
			CurrentVector = 1;
			CurrentTetromino[3][0] = 2;
			CurrentTetromino[3][1] = 2;
		}
		else if (CurrentVector == 1)
		{
			CurrentVector = 2;
			CurrentTetromino[2][0] = 3;
			CurrentTetromino[2][1] = 1;
		}
		else if (CurrentVector == 2)
		{
			CurrentVector = 3;
			CurrentTetromino[0][0] = 2;
			CurrentTetromino[0][1] = 0;
		}
		else if (CurrentVector == 3)
		{
			CurrentVector = 0;
			CurrentTetromino[0][0] = 1;
			CurrentTetromino[0][1] = 1;
			CurrentTetromino[2][0] = 2;
			CurrentTetromino[2][1] = 0;
			CurrentTetromino[3][0] = 3;
			CurrentTetromino[3][1] = 1;
		}
		break;
	case 4:
		//J
		if (CurrentVector == 0)
		{
			CurrentVector = 1;
			CurrentTetromino[2][0] = 0;
			CurrentTetromino[2][1] = 2;
			CurrentTetromino[3][0] = 1;
			CurrentTetromino[3][1] = 2;
		}
		else if (CurrentVector == 1)
		{
			CurrentVector = 2;
			CurrentTetromino[0][0] = 2;
			CurrentTetromino[0][1] = 2;
			CurrentTetromino[1][0] = 2;
			CurrentTetromino[1][1] = 3;
		}
		else if (CurrentVector == 2)
		{
			CurrentVector = 3;
			CurrentTetromino[2][0] = 2;
			CurrentTetromino[2][1] = 1;
			CurrentTetromino[3][0] = 3;
			CurrentTetromino[3][1] = 1;
		}
		else if (CurrentVector == 3)
		{
			CurrentVector = 0;
			CurrentTetromino[0][0] = 1;
			CurrentTetromino[0][1] = 0;
			CurrentTetromino[1][0] = 1;
			CurrentTetromino[1][1] = 1;
		}
		break;
	case 5:
		//L
		if (CurrentVector == 0)
		{
			CurrentVector = 1;
			CurrentTetromino[2][0] = 2;
			CurrentTetromino[2][1] = 2;
			CurrentTetromino[3][0] = 2;
			CurrentTetromino[3][1] = 3;
		}
		else if (CurrentVector == 1)
		{
			CurrentVector = 2;
			CurrentTetromino[2][0] = 0;
			CurrentTetromino[2][1] = 1;
			CurrentTetromino[3][0] = 0;
			CurrentTetromino[3][1] = 2;
		}
		else if (CurrentVector == 2)
		{
			CurrentVector = 3;
			CurrentTetromino[0][0] = 0;
			CurrentTetromino[0][1] = 3;
			CurrentTetromino[1][0] = 1;
			CurrentTetromino[1][1] = 3;
		}
		else if (CurrentVector == 3)
		{
			CurrentVector = 0;
			for (int i = 0; i < 4; i++)
			{
				int BasicBlock = Types[CurrentType][i];
				CurrentTetromino[i][0] = BasicBlock / 2;	//col
				CurrentTetromino[i][1] = BasicBlock % 2;	//row
			}
		}
		break;
	case 6:
		//O
		break;
	default:
		break;
	}
}

void AKKAITetris::resetMap()
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Map[i][j] = TempMap[i][j];
		}
	}
}
void AKKAITetris::setTempMap()
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			TempMap[i][j] = Map[i][j];
		}
	}
}
void AKKAITetris::setFinalMap()
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			FinalMap[i][j] = Map[i][j];
		}
	}
}
void AKKAITetris::setMap()
{
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			Map[i][j] = FinalMap[i][j];
		}
	}
}

void AKKAITetris::setAICopyMap()
{
	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (i == 0 || j == 0 || i == 21 || j == 11)
			{
				CopyMap[i][j] = 1;
				continue;
			}
			else
			{
				CopyMap[i][j] = Map[i - 1][j - 1];
			}
		}
	}
}

//碰撞检查，防止方块重叠或者越界
bool AKKAITetris::checkCollision(int T[4][2])
{
	for (int i = 0; i < 4; i++)
	{
		int col = T[i][0];
		int row = T[i][1];
		if (col < 0 || col > 9 || row > 19 || Map[row][col] != 0)
		{
			return true;
		}
	}
	return false;
}

//AI计算
void AKKAITetris::AIOperation()
{
	MaxResult = -0x3f3f3f;
	setTempMap();
	setTempTetromino();

	//深度优先搜索，检索素所有可降落的位置
	dfs(CurrentTetromino);
	rotate();
	dfs(CurrentTetromino);
	rotate();
	dfs(CurrentTetromino);
	rotate();
	dfs(CurrentTetromino);
}

void AKKAITetris::calLHandRC(int T[4][2])
{
	int topHeight = T[0][1];
	int bottomHeight = T[3][1];
	if (topHeight < bottomHeight)
	{
		Swap(topHeight, bottomHeight);
	}
	LandingHeight = 19 - (topHeight + bottomHeight) / 2;

	int numGrid = 0;
	int numLine = 0;
	TSet<int> HashTableGrid;
	TSet<int> HashTableLine;
	for (int i = 0; i < 4; i++)
	{
		int row = T[i][1];
		for (int j = 0; j < 10; j++)
		{
			if (Map[row][j] == 0)
			{
				break;
			}
			else if (j == 9)
			{
				HashTableGrid.Emplace(i);
				HashTableLine.Emplace(row);
			}
		}
	}
	numGrid = HashTableGrid.GetAllocatedSize();
	numLine = HashTableLine.GetAllocatedSize();
	RowCleared = numGrid * numLine;
}

void AKKAITetris::AICalculation(int T[4][2])
{
	//计算LH和RC
	//消行，更新Map
	//复制Map
	//计算其余四个参数
	//求出PD值
	//对比PD值，如果大于当前max，则用当前的Map替代FinalMap
	calLHandRC(T);
	clearFullLine();
	setAICopyMap();
	calCT();
	calHS();
	calRT();
	calWS();
	float result = PierreDellacherieFormule();
	if (result > MaxResult)
	{
		MaxResult = result;
		setFinalMap();
	}
}

void AKKAITetris::setTempTetromino()
{
	for (int i = 0; i < 4; i++)
	{
		TempTetromino[i][0] = CurrentTetromino[i][0];
		TempTetromino[i][1] = CurrentTetromino[i][1];
	}
}

void AKKAITetris::resetCurrentTetromino()
{
	for (int i = 0; i < 4; i++)
	{
		CurrentTetromino[i][0] = TempTetromino[i][0];
		CurrentTetromino[i][1] = TempTetromino[i][1];
	}
}

void AKKAITetris::calRT()
{
	RowTransitions = 0;
	for (int i = 1; i < 21; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			if (!(CopyMap[i][j] && CopyMap[i][j + 1]) && (CopyMap[i][j] || CopyMap[i][j + 1]))
			{
				RowTransitions++;
			}
		}
	}
}

void AKKAITetris::calCT()
{
	ColTransitions = 0;
	for (int j = 1; j < 11; j++)
	{
		for (int i = 0; i < 21; i++)
		{
			if (!(CopyMap[i][j] && CopyMap[i + 1][j]) && (CopyMap[i][j] || CopyMap[i + 1][j]))
			{
				ColTransitions++;
			}
		}
	}
}

void AKKAITetris::calHS()
{
	HoleSums = 0;
	bool flag = false;
	for (int j = 1; j < 11; j++)
	{
		flag = false;
		for (int i = 1; i < 21; i++)
		{
			if (CopyMap[i][j] != 0)
			{
				flag = true;
			}
			else
			{
				if (flag)
				{
					HoleSums++;
					flag = false;
				}
			}
		}
	}
}

void AKKAITetris::calWS()
{
	WellSums = 0;
	int num = 0;
	for (int j = 1; j < 11; j++)
	{
		for (int i = 1; i < 21; i++)
		{
			if (!CopyMap[i][j] && CopyMap[i][j - 1] && CopyMap[i][j + 1])
			{
				num++;
			}
			else
			{
				WellSums += calDec(num);
				num = 0;
			}
		}
	}
}

int AKKAITetris::calDec(int num)
{
	if (num == 0 || num == 1)
	{
		return num;
	}
	int res = 1;
	for (int i = num; i > 1; i--)
	{
		res += i;
	}
	return res;
}

float AKKAITetris::PierreDellacherieFormule()
{
	int result =
		-4.500 * LandingHeight
		+ 3.418 * RowCleared
		- 3.217 * RowTransitions
		- 9.348 * ColTransitions
		- 7.899 * HoleSums
		- 3.385 * WellSums;
	return result;
}

void AKKAITetris::dfs(int T[4][2])
{
	//备份，防止越界
	int Prev[4][2];
	for (int i = 0; i < 4; i++)
	{
		Prev[i][0] = T[i][0];
		Prev[i][1] = T[i][1];
	}
	//移动到最右端
	while (!checkCollision(T))
	{
		//更新Prev
		for (int i = 0; i < 4; i++)
		{
			Prev[i][0] = T[i][0];
			Prev[i][1] = T[i][1];
		}
		//移动T
		for (int i = 0; i < 4; i++)
		{
			T[i][0]--;
		}
	}
	//此时Prev在最右端，将其更新到Tetromino并开始计算PD
	//Prev开始左移
	while (!checkCollision(Prev))
	{
		fall(Prev);
		//for (int i = 0; i < 4; i++)
		//{
		//	T[i][0] = Prev[i][0];
		//	T[i][1] = Prev[i][1];
		//}
		for (int i = 0; i < 4; i++)
		{
			Prev[i][0]++;
		}
	}
}

void AKKAITetris::fall(int Prev[4][2])
{
	//fall down
	for (int i = 0; i < 4; i++)
	{
		CurrentTetromino[i][0] = Prev[i][0];
		CurrentTetromino[i][1] = Prev[i][1];
	}
	for (int i = 0; i < 19; i++)
	{
		moveDown();
	}
	AICalculation(CurrentTetromino);
	resetMap();
	resetCurrentTetromino();
}

