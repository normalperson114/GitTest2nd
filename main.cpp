
#include "main.h"



struct VERTEX_3D
{
	Float3 Position;
	Float4 Color;
	Float2 TexCoord;
};




extern "C" void nnMain()
{
	InitSystem();

	// 後ろ向きを描画しない（通常設定）
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// 頂点が反時計回りならば前（通常設定）
	glFrontFace(GL_CCW);

	while( true )
	{
		glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		VERTEX_3D vertex[4];

		vertex[0].Position = MakeFloat3(+100.0f, -100.0f, 0.0f);
		vertex[1].Position = MakeFloat3(-100.0f, -100.0f, 0.0f);
		vertex[2].Position = MakeFloat3(+100.0f, +100.0f, 0.0f);
		vertex[3].Position = MakeFloat3(-100.0f, +100.0f, 0.0f);

		vertex[0].Color = MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].Color = MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].Color = MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].Color = MakeFloat4(1.0f, 1.0f, 1.0f, 1.0f);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Position);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->Color);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX_3D), (GLvoid*)&vertex->TexCoord);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);



		SwapBuffers();
	}

	UninitSystem();
}




