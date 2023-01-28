Shader "Unlit/DepthTextureMipmapCalculator"
{
    Properties
    {
       [HideInInspector] _MainTex ("Texture", 2D) = "black" {}
    }
    SubShader
    {
        Tags { "RenderType"="Opaque" }
        LOD 100

        Pass
        {
            ZTest Always
            ZWrite Off
            Cull Off

            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag

            #include "UnityCG.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct v2f
            {
                float2 uv : TEXCOORD0;
                float4 vertex : SV_POSITION;
            };

            sampler2D _MainTex;
            float4 _MainTex_TexelSize;

            v2f vert (appdata v)
            {
                v2f o;
                o.vertex = UnityObjectToClipPos(v.vertex);
                o.uv = v.uv;
                return o;
            }

            inline float CalculatorMipmapDepth(float2 uv)
            {
                float4 depth;
                // sample the texture
                float offset = _MainTex_TexelSize.x * 0.5;
                // 这里的x应该是currentTexture的1/w;
                depth.x = tex2D(_MainTex, uv);
                depth.y = tex2D(_MainTex, uv+float2(offset,0));
                depth.z = tex2D(_MainTex, uv+float2(0,offset));
                depth.w = tex2D(_MainTex, uv+float2(offset,offset));
                #ifdef UNITY_REVERSED_Z
                    return max(depth.x, max(depth.y, max(depth.z, depth.w)));
                #else
                    return min(depth.x, min(depth.y, min(depth.z, depth.w)));
                #endif
            }

            float4 frag (v2f i) : SV_Target
            {
                float depth =  CalculatorMipmapDepth(i.uv);
                return float4(depth, 0, 0 , 1.0f);
            }
            ENDCG
        }
    }
}
