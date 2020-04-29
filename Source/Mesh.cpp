#include "Mesh.h"

void jkMesh::CalcTangentSpace(bool mirror)
{
	for (size_t i = 0; i < mIndexBuffer.size(); i+=3)
	{
		UINT i1 = mIndexBuffer.at(i);
		UINT i2 = mIndexBuffer.at(i+1);
		UINT i3 = mIndexBuffer.at(i+2);

		auto& v1 = mVertexBuffer.at(i1);
		auto& v2 = mVertexBuffer.at(i2);
		auto& v3 = mVertexBuffer.at(i3);

		auto e1 = v2.pos - v1.pos;
		auto e2 = v3.pos - v1.pos;

		auto deltaUV1 = v2.texcoord - v1.texcoord;
		auto deltaUV2 = v3.texcoord - v1.texcoord;

		/*if (mirror && (v1.pos.x < 0 || v2.pos.x < 0 || v3.pos.x < 0))
		{
			deltaUV1 = { -deltaUV1.x, deltaUV1.y };
			deltaUV2 = { -deltaUV2.x, deltaUV2.y };
		}*///Do't work.

		/*
		[E1x, E1y, E1z  =  [¦¤U1, ¦¤V1  * [Tx, Ty, Tz
		 E2x, E2y, E2z]     ¦¤U2, ¦¤V2]    Bx, By, Bz]
		*/
		float oneDivDet = 1.f / (deltaUV1.x * deltaUV2.y
			- deltaUV2.x * deltaUV1.y);

		VEC3 tangent;
		tangent.x = oneDivDet
			* (deltaUV2.y * e1.x - deltaUV1.y * e2.x);
		tangent.y = oneDivDet
			* (deltaUV2.y * e1.y - deltaUV1.y * e2.y);
		tangent.z = oneDivDet
			* (deltaUV2.y * e1.z - deltaUV1.y * e2.z);
		tangent = tangent.Normalize();
		v1.tangent = (v1.tangent + tangent).Normalize();
		v2.tangent = (v2.tangent + tangent).Normalize();
		v3.tangent = (v3.tangent + tangent).Normalize();
		//v1.tangent = {1.f, 0.f, 0.f};
		//v2.tangent = { 1.f, 0.f, 0.f };
		//v3.tangent = { 1.f, 0.f, 0.f };
		
		VEC3 bitangent;
		bitangent.x = oneDivDet
			* (-deltaUV2.x * e1.x + deltaUV1.x * e2.x);
		bitangent.y = oneDivDet
			* (-deltaUV2.x * e1.y + deltaUV1.x * e2.y);
		bitangent.z = oneDivDet
			* (-deltaUV2.x * e1.z + deltaUV1.x * e2.z);
		bitangent = bitangent.Normalize();
		v1.bitangent = (v1.bitangent + bitangent).Normalize();
		v2.bitangent = (v2.bitangent + bitangent).Normalize();
		v3.bitangent = (v3.bitangent + bitangent).Normalize();
		//v1.bitangent = { 0.f, 0.f, -1.f };
		//v2.bitangent = {0.f, 0.f, -1.f};
		//v3.bitangent = {0.f, 0.f, -1.f};

		if (mirror && (v1.pos.x < 0 || v2.pos.x < 0 || v3.pos.x < 0))
		{
			
				v1.tangent = -v1.tangent;
				v2.tangent = -v2.tangent;
				v3.tangent = -v3.tangent;
			
		}
		
	}
}

void jkMesh::mFunction_UpdateRotateMatrix()
{
	mRotateMatrix = 
		GetRotationMatrix_Euler(GetRadian(mRotationPitch), GetRadian(mRotationYaw), GetRadian(mRotationRoll));
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
	mWorldMatrix = mScaleTranslateMatrix * mRotateMatrix;//First rotate then translate.
}
