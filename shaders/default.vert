#version 330 core

layout (location = 0) in vec3 a_Position;  // 顶点位置
layout (location = 1) in vec4 a_Color;     // 顶点颜色（支持染色）
layout (location = 2) in vec2 a_TexCoord;  // 纹理 UV 坐标
layout (location = 3) in float a_TexIndex; // 纹理槽位索引

uniform mat4 u_ViewProjection; // 摄像机投影矩阵

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    
    // 最终位置 = 投影矩阵 * 顶点坐标
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}