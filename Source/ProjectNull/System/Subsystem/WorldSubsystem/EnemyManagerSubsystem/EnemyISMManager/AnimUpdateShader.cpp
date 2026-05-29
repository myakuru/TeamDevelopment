#include "AnimUpdateShader.h"

// シェーダーの実装登録
IMPLEMENT_GLOBAL_SHADER(FApplyChangeRequestCS, "/ProjectNull/AnimUpdate/AnimUpdate_ApplyChangeRequest.usf", "ApplyChangeRequestCS", SF_Compute);
IMPLEMENT_GLOBAL_SHADER(FAnimUpdateCS, "/ProjectNull/AnimUpdate/AnimUpdate_Main.usf", "MainCS", SF_Compute);
