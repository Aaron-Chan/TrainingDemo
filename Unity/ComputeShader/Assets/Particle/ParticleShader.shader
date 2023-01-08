Shader "Unlit/ParticleShader"
{
    Properties
    {
        _MainTex ("Texture", 2D) = "white" {}
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

           

            struct v2f
            {
                float4 color : COLOR0;
                float4 vertex : SV_POSITION;
            };

            sampler2D _MainTex;
            float4 _MainTex_ST;

            struct ParticleData 
            {
                float3 position;
                float4 color;
            };

            StructuredBuffer<ParticleData> _particleDataBuffer;

            v2f vert (uint id : SV_VertexID)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(float4(_particleDataBuffer[id].position,0));
                o.color = _particleDataBuffer[id].color;
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                return i.color;
            }
            ENDCG
        }
    }
}
