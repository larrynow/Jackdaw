#include "Mesh.h"

void jkMesh::mFunction_UpdateRotateMatrix()
{
	mRotateMatrix = Matrix_RotationXYZ(GetRadian(mRotationPitch), GetRadian(mRotationYaw), GetRadian(mRotationRoll));
}

void jkMesh::mFunction_UpdateScaleMatrix()
{
	mScaleTranslateMatrix.m[0][0] = mScaleX;
	mScaleTranslateMatrix.m[1][1] = mScaleY;
	mScaleTranslateMatrix.m[2][2] = mScaleZ;
}

void jkMesh::mFunction_UpdateTranslateMatrix()
{
	MakeTranslateMatrix(mScaleTranslateMatrix, mPosition.x, mPosition.y, mPosition.z);
	//mScaleTranslateMatrix.m[0][3] = mPosition.x;
	//mScaleTranslateMatrix.m[1][3] = mPosition.y;
	//mScaleTranslateMatrix.m[2][3] = mPosition.z;
}

void jkMesh::mFunction_UpdateWorldMatrix()
{
	mWorldMatrix = mScaleTranslateMatrix * mRotateMatrix;
}
