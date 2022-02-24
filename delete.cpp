#include "obstacle.h"
#include "Fever.h"
#include "StageSelect.h"
#include "Player.h"
#include "Texture.h"
#include "sprite.h"
#include "TimeLimit.h"
#include "Rhythm.h"
#include "Judgement.h"
#include "score.h"
#include <random>
#include <time.h>

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
static bool RandomFlag(float probability);
void PopObstacle(E_OBSTACLE_KIND o);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int			g_rock_tex_num;						//テクスチャ
static int			g_bom_tex_num;						//テクスチャ
static int			g_shark_tex_num;					//テクスチャ

static bool				g_LR_Judge_Flag_Obstacle;		//Useフラグ
static E_OBSTACLE_KIND	g_Obstacle_Flag;				//全部フラグ
static int				g_Pop_time;		//障害物の発生時間
static bool				g_Obstacle_Hit_Flag;			//障害物に当たったかどうか　true:当たった

static float		g_obstacle_pos_x;					//X座標
static float		g_obstacle_pos_y;					//Y座標
static float		g_obstacle_vel_x;					//加速度

static	StageSelectData		g_stage_data_obstacle;		//ステージデータ格納
static bool					g_Ferver_Flag_obstacle;		//フィーバーフラグ格納
static s_PLAYER*			g_s_player_obstacle;        //プレイヤーの各データ格納


//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitObstacle(void)
{
	g_rock_tex_num  =	LoadTexture("data/TEXTURE/rock.png");
	g_shark_tex_num =	LoadTexture("data/TEXTURE/shark.png");

	g_LR_Judge_Flag_Obstacle = false;
	g_Obstacle_Hit_Flag = false;
	g_Pop_time = -1;

	g_Obstacle_Flag = E_OBSTACLE_KIND::MAX_E_OBSTACLE;

	g_stage_data_obstacle  = GetStageSelectData();
	g_Ferver_Flag_obstacle = GetFeverFlag();
	g_s_player_obstacle	   = GetPlayer();

	g_obstacle_pos_x	= (SCREEN_WIDTH  + (OBSTACLE_TEXTURE_WIDTH  / 2));
	g_obstacle_pos_y = g_s_player_obstacle->m_pos.y;
	g_obstacle_vel_x = -5.0f;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitObstacle(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateObstacle(void)
{
	g_LR_Judge_Flag_Obstacle = GetLRJudgeFlag();
	
	//使用フラグで岩もサメも使っていない時だけ処理する
	//OR条件になってたぞ貴様ぁ!!(°Д°)
	if (g_Obstacle_Flag != E_OBSTACLE_KIND::ROCK && g_Obstacle_Flag != E_OBSTACLE_KIND::SHARK)
	{
		//ステージ7,8で使うやよ～
		switch (g_stage_data_obstacle.m_stage_num)
		{
		case E_STAGE::STAGE7:
			switch (g_stage_data_obstacle.m_difficulty_level)
			{
			case E_DIFFICULTY_LEVEL::EASY:
				if (g_LR_Judge_Flag_Obstacle)
				{
					if (RandomFlag(0.5f))
					{
						PopObstacle(E_OBSTACLE_KIND::ROCK);
					}
				}
				break;

			case E_DIFFICULTY_LEVEL::NOMAL:
				if (g_LR_Judge_Flag_Obstacle)
				{
					if (RandomFlag(0.5f))
					{
						PopObstacle(E_OBSTACLE_KIND::SHARK);
						
					}
				}
				break;

			case E_DIFFICULTY_LEVEL::HARD:
				if (g_LR_Judge_Flag_Obstacle)
				{
					if (RandomFlag(0.5f))
					{
						if (RandomFlag(0.5f))
						{
							PopObstacle(E_OBSTACLE_KIND::ROCK);
						}
						else
						{
							PopObstacle(E_OBSTACLE_KIND::SHARK);
						}
					}
				}
				break;

			default:
				break;
			}
			break;

		case E_STAGE::STAGE8:
			switch (g_stage_data_obstacle.m_difficulty_level)
			{
			case E_DIFFICULTY_LEVEL::EASY:
				if (g_LR_Judge_Flag_Obstacle)
				{
					if (RandomFlag(0.5f))
					{
						PopObstacle(E_OBSTACLE_KIND::ROCK);
					}
				}
				break;

			case E_DIFFICULTY_LEVEL::NOMAL:

				break;

			case E_DIFFICULTY_LEVEL::HARD:
				if (g_LR_Judge_Flag_Obstacle)
				{
					if (RandomFlag(0.5f))
					{
						if (RandomFlag(0.5f))
						{
							PopObstacle(E_OBSTACLE_KIND::ROCK);
						}
						else
						{
							PopObstacle(E_OBSTACLE_KIND::SHARK);
						}
					}
				}
				break;

			default:

				break;
			}

			//それ以外のステージでは使わないやよ～
		default:

			break;
		}

		if (g_Obstacle_Flag != E_OBSTACLE_KIND::MAX_E_OBSTACLE && g_Ferver_Flag_obstacle != true)
		{
			//一応バグ怖いから
			if (g_Obstacle_Flag != E_OBSTACLE_KIND::BOMB)
			{
				Obstacle();
			}
			
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawObstacle(void)
{
	switch (g_Obstacle_Flag)
	{
	case E_OBSTACLE_KIND::ROCK:
		DrawSprite(g_rock_tex_num, g_obstacle_pos_x, g_obstacle_pos_y,
			OBSTACLE_TEXTURE_WIDTH, OBSTACLE_TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
		break;

	case E_OBSTACLE_KIND::SHARK:
		DrawSprite(g_shark_tex_num, g_obstacle_pos_x, g_obstacle_pos_y,
			OBSTACLE_TEXTURE_WIDTH, OBSTACLE_TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
		break;

	default:
		break;
	}
}

void Obstacle(void)
{
	
	//画面端で戻す＆フラグをMAXに
	if (g_obstacle_pos_x + OBSTACLE_TEXTURE_WIDTH / 2 < 0)
	{
		g_obstacle_pos_x = SCREEN_WIDTH + OBSTACLE_TEXTURE_WIDTH / 2;
		g_Obstacle_Flag = E_OBSTACLE_KIND::MAX_E_OBSTACLE;
		g_Obstacle_Hit_Flag = false;
	}
	else//画面端じゃない時
	{

		//フィーバーの時はfalseにする
		if (g_Ferver_Flag_obstacle == true)
		{
			g_Obstacle_Flag = E_OBSTACLE_KIND::MAX_E_OBSTACLE;
		}

		//MAXじゃない時移動させる
		if (g_Obstacle_Flag != E_OBSTACLE_KIND::MAX_E_OBSTACLE)
		{
			g_obstacle_pos_x += g_obstacle_vel_x;
		}
		//コマンドのミス＝衝突なので当たり判定とかはいらないです
		//コマンドミス→消して減算の処理
	}
	
	//ONE_GRID_TIME * (DEBUG_BEAT + 1)
	//150 - G_TIME/2   回避トリックの開始フレーム
	//150 - (G_TIME - 1)/2   回避トリックの終了フレーム

}

static bool RandomFlag(float probability)
{
	// 乱数生成器（引数にシードを指定）
	static std::default_random_engine engine((unsigned int)time(NULL));

	// 引数値の確率でのベルヌーイ分布(bool型)の分布生成器
	std::bernoulli_distribution get_rand_uni_bool(probability);

	// 乱数を生成
	return get_rand_uni_bool(engine);
}

void HitObatacle(void)//障害物に衝突するかの判定
{
	int l_ElapsedFrameObstacle = GetElapsedFrameObstacle();
	if (l_ElapsedFrameObstacle > (150 - G_TIME / 2) && l_ElapsedFrameObstacle < 150 + (G_TIME - 1) / 2)
	{

		g_Obstacle_Hit_Flag = true;
		AddScore(InshdexScoreObstacle(g_stage_data_obstacle.m_difficulty_level, g_Obstacle_Flag));

		//衝突時のエフェクト描画
	}

}

int GetElapsedFrameObstacle(void)//障害物発生からの経過時間を返す
{
	return GetFrameCountJudge() - g_Pop_time;
}

void PopObstacle(E_OBSTACLE_KIND o)
{
	g_Obstacle_Flag = o;
	g_Pop_time = GetFrameCountJudge();

}
