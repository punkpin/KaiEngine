#version 330 core

layout (location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

// 预留 32 个纹理槽位（现代显卡基本都支持）
uniform sampler2D u_Textures[32];

void main()
{
    // 根据索引选择对应的纹理进行采样
    int index = int(v_TexIndex);
    vec4 texColor = texture(u_Textures[index], v_TexCoord);
    
    // 最终颜色 = 贴图颜色 * 顶点颜色 (支持透明度和染色)
    o_Color = texColor * v_Color;
    
    // 如果是纯色方块（白贴图），结果就是 v_Color
    // 如果是贴图，结果就是贴图原色（v_Color 为白色时）
}