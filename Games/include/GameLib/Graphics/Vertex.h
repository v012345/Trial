#ifndef INCLUDED_GAMELIB_GRAPHICS_VERTEX_H
#define INCLUDED_GAMELIB_GRAPHICS_VERTEX_H

#include "GameLib/Math/Vector4.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Vector2.h"

namespace GameLib{
using namespace Math;
namespace Graphics{

class Vertex{
public:
	Vector4 mPosition; //位置
	Vector3 mNormal; //法线
	unsigned mColor; //顶点颜色
	Vector2 mUv; //纹理坐标
};

} //namespace Graphics
} //namespace GameLib

#endif
