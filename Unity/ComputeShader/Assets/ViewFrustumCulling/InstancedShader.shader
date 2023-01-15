Shader "Unlit/InstancedShader"
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
            // make fog work
            #pragma multi_compile_fog
            #pragma target 4.5

            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                UNITY_FOG_COORDS(1)
                float4 vertex : SV_POSITION;
            };

            sampler2D _MainTex;
            float4 _MainTex_ST;

             #if SHADER_TARGET >= 45
                StructuredBuffer<float4x4> drawPositionBuffer;
            #endif
            

            v2f vert (appdata v, uint instanceID : SV_InstanceID)
            {
                 #if SHADER_TARGET >= 45
                    float4x4 data = drawPositionBuffer[instanceID];
                #else
                    float4x4 data = 0;
                #endif
                float3 localPosition = v.vertex.xyz * data._11;
                float3 worldPosition = data._14_24_34 + localPosition;
                v2f o;
                o.vertex = mul(UNITY_MATRIX_VP, float4(worldPosition, 1.0));
                o.uv = TRANSFORM_TEX(v.uv, _MainTex);
                UNITY_TRANSFER_FOG(o,o.vertex);
                return o;
            }

            fixed4 frag (v2f i) : SV_Target
            {
                // sample the texture
                fixed4 col = tex2D(_MainTex, i.uv);
                // apply fog
                UNITY_APPLY_FOG(i.fogCoord, col);
                return col;
            }
            ENDCG
        }
    }
}
