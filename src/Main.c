//#include "C:/Wichtig/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"

#define MAX_VALUES	10000
#define MAX_LENGTH	1000

float fBTime;
int nMaxIterations;
float fRotationSpeed;
float fOffset;
TransformedView tv;
Vector vVales;

void Setup(AlxWindow* w){
	fBTime = 0.0f;
	nMaxIterations = 1;
	fRotationSpeed = 1.0f;
	fOffset = 4.0f;
	tv = TransformedView_New((Vec2){ GetWidth(),GetHeight() });
	vVales = Vector_New(sizeof(float));
}
void Update(AlxWindow* w){
	TransformedView_HandlePanZoom(&tv,window.Strokes,GetMouse());
	Rect r = TransformedView_Rect(&tv,GetScreenRect());

	fBTime += w->ElapsedTime;

	if(Stroke(ALX_KEY_W).PRESSED) nMaxIterations++;
	if(Stroke(ALX_KEY_S).PRESSED) nMaxIterations--;

	Clear(BLACK);

	Vec2 prev = { 0.0f,0.0f };
	for(int i = 0;i<nMaxIterations;i++){
		int n = i * 2 + 1;
		
		float a = n * fRotationSpeed * fBTime;
		float r = (4.0f / (F32_PI * (float)n));
		Vec2 target = Vec2_Add(prev,Vec2_Mulf(Vec2_OfAngle(a),r));

		Vec2 pS = TransformedView_WorldScreenPos(&tv,prev);
		Vec2 tS = TransformedView_WorldScreenPos(&tv,target);
		Line_RenderX(WINDOW_STD_ARGS,pS,tS,WHITE,1.0f);

		float rS = TransformedView_WorldScreenLX(&tv,r);
		Circle_RenderXWire(WINDOW_STD_ARGS,pS,rS,WHITE,1.0f);

		prev = target;
	}
	
	Vector_Add(&vVales,&prev.y,0);
	if(vVales.size>MAX_VALUES) Vector_PopTop(&vVales);

	for(int i = 1;i<vVales.size;i++){
		float v0 = *(float*)Vector_Get(&vVales,i-1);
		float v1 = *(float*)Vector_Get(&vVales,i);

		Vec2 pS = TransformedView_WorldScreenPos(&tv,(Vec2){ fOffset + (float)(i-1	) / (float)MAX_LENGTH,v0 });
		Vec2 tS = TransformedView_WorldScreenPos(&tv,(Vec2){ fOffset + (float)(i	) / (float)MAX_LENGTH,v1 });
		Line_RenderX(WINDOW_STD_ARGS,pS,tS,WHITE,1.0f);
	}

	String str = String_Format("MI:%d",nMaxIterations);
	CStr_RenderSizeAlxFont(WINDOW_STD_ARGS,&window.AlxFont,str.Memory,str.size,0.0f,0.0f,WHITE);
	String_Free(&str);
}
void Delete(AlxWindow* w){
	Vector_Free(&vVales);
}

int main(){
    if(Create("FTransform",1200,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}