#include "OBB2.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/EngineCommon.hpp"

OBB2::OBB2()
{
    m_center = Vec2(0, 0);
    m_iBasisNormal = Vec2(1, 0);
    m_halfDimensions = Vec2(0.0f,0.0f);
}
OBB2::OBB2(Vec2 center, Vec2 normal, Vec2 halfDimensions)
{
    m_center = center;
    m_iBasisNormal = normal;
    m_halfDimensions = halfDimensions; 
}
OBB2::~OBB2()
{
}
void OBB2::GetCornerPoints(Vec2* out_fourCornerWorldPositios) const
{
    UNUSED((void)out_fourCornerWorldPositios);
}

Vec2 OBB2::GetLocalPosForWorldPos(Vec2 worldPos) const
{
    Vec2 returnValue = Vec2(worldPos.x - m_center.x / m_iBasisNormal.x, worldPos.y - m_center.y / m_iBasisNormal.y);
    return returnValue;
}

Vec2 OBB2::GetWorldPosForLocalPos(Vec2 localPos) const
{
    Vec2 returnValue = Vec2(m_center.x + m_iBasisNormal.x * localPos.x, m_center.y + m_iBasisNormal.y * localPos.y);
    return returnValue;
}

void OBB2::RotateAboutCenter(float rotationDeltaDegrees)
{
    float orientation = Atan2Degrees(m_iBasisNormal.y, m_iBasisNormal.x);
    orientation += rotationDeltaDegrees;
    m_iBasisNormal = Vec2(CosDegrees(orientation), SinDegrees(orientation));
}
