#pragma once

#define USE_TEXCOORD0
#define USE_COLOR

#include <Shaders/Common/GlobalConstants.h>
#include <Shaders/Materials/MaterialInterpolator.h>
#include <Shaders/Particles/ParticleSystemConstants.h>

static float2 QuadTexCoords[6] =
{
  float2(0.0, 0.0),
  float2(1.0, 0.0),
  float2(1.0, 1.0),
  float2(0.0, 0.0),
  float2(1.0, 1.0),
  float2(0.0, 1.0),
};

uint CalcQuadParticleDataIndex(uint VertexID)
{
  return VertexID / 6;
}

uint CalcQuadParticleVertexIndex(uint VertexID)
{
  return VertexID % 6;
}

float4 CalcQuadOutputPositionWithTangents(uint vertexIndex, float3 inPosition, float3 inTangentX, float3 inTangentZ, float inSize)
{
  float4 position = float4(inPosition, 1);

  float4 tangentX = float4(inTangentX, 0);
  float4 tangentZ = float4(inTangentZ, 0);

  float4 offsetRight = tangentX * (QuadTexCoords[vertexIndex].x - 0.5) * inSize;
  float4 offsetUp = tangentZ * (QuadTexCoords[vertexIndex].y - 0.5) * -inSize;

  float4 worldPosition = mul(ObjectToWorldMatrix, position + offsetRight + offsetUp);
  float4 screenPosition = mul(GetWorldToScreenMatrix(), worldPosition);

  return screenPosition;
}

float3x3 CreateRotationMatrixY(float radians)
{
  float fsin = sin(radians);
  float fcos = cos(radians);

  return float3x3(fcos, 0, fsin, 0, 1, 0, -fsin, 0, fcos);
}

float4 CalcQuadOutputPositionAsBillboard(uint vertexIndex, float3 centerPosition, float rotationOffset, float rotationSpeed, float inSize)
{
  //float4 position = TransformToPosition(inTransform);
  float4 position = float4(centerPosition, 1);
  //float3x3 rotation = TransformToRotation(inTransform);
  float3x3 rotation = CreateRotationMatrixY(rotationOffset + rotationSpeed * WorldTime);

  float3 offsetRight = GetCameraDirRight() * (QuadTexCoords[vertexIndex].x - 0.5) * inSize;
  float3 offsetUp = GetCameraDirUp() * (QuadTexCoords[vertexIndex].y - 0.5) * -inSize;

  float3 offsetRightCS = mul(GetWorldToCameraMatrix(), float4(offsetRight, 0)).xzy;
  float3 offsetUpCS = mul(GetWorldToCameraMatrix(), float4(offsetUp, 0)).xzy;

  offsetRightCS = mul(rotation, offsetRightCS);
  offsetUpCS = mul(rotation, offsetUpCS);

  float4 worldPosition = mul(ObjectToWorldMatrix, position);
  float4 cameraPosition = mul(GetWorldToCameraMatrix(), worldPosition);
  cameraPosition.xzy = cameraPosition.xzy + offsetRightCS + offsetUpCS;
  float4 screenPosition = mul(GetCameraToScreenMatrix(), cameraPosition);

  return screenPosition;
}

float2 ComputeSpriteAnimationTexCoord(float2 baseTexCoord, uint numVarsX, uint numVarsY, float varLerp, uint numAnimsX, uint numAnimsY, float animLerp)
{
  float2 texCoordSize = float2(1, 1);
  float2 texCoordOffset = float2(0, 0);

  if (numVarsX > 1 || numVarsY > 1)
  {
    uint numVars = numVarsX * numVarsY;
    uint idxVar = (uint)(numVars * varLerp);
    uint varY = idxVar / numVarsX;
    uint varX = (idxVar - (varY * numVarsX));
    texCoordSize = texCoordSize / float2(numVarsX, numVarsY);
    texCoordOffset = texCoordOffset + texCoordSize * float2(varX, varY);
  }

  if (numAnimsX > 1 || numAnimsY > 1)
  {
    uint numAnims = numAnimsX * numAnimsY;
    uint idxAnim = (uint)(numAnims * animLerp);
    uint animY = idxAnim / numAnimsX;
    uint animX = (idxAnim - (animY * numAnimsX));
    texCoordSize = texCoordSize / float2(numAnimsX, numAnimsY);
    texCoordOffset = texCoordOffset + texCoordSize * float2(animX, animY);
  }

  return texCoordOffset + baseTexCoord * texCoordSize;
}


