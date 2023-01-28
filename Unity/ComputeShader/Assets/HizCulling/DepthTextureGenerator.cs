using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DepthTextureGenerator : MonoBehaviour
{
	RenderTexture m_depthTexture;//带 mipmap 的深度图
	public RenderTexture depthTexture => m_depthTexture;

	int CameraDepthTextureId;

	public Shader HizDepthTextureShader;
	Material HizDepthTextureMat;
	const RenderTextureFormat DepthTextureFormat = RenderTextureFormat.RHalf;

	public int depthSize() {
		return Mathf.NextPowerOfTwo(Mathf.Max(Screen.width, Screen.height));
	}

	private void Start()
	{
		Camera.main.depthTextureMode |= DepthTextureMode.Depth;
		CameraDepthTextureId = Shader.PropertyToID("_CameraDepthTexture");
		HizDepthTextureMat = new Material(HizDepthTextureShader);
		InitDepthTexture();
	}

	void InitDepthTexture() {
		if (m_depthTexture != null) return;
		int size = depthSize();
		m_depthTexture = new RenderTexture(size, size, 0, DepthTextureFormat);
		m_depthTexture.useMipMap = true;
		m_depthTexture.autoGenerateMips = false;
		m_depthTexture.filterMode = FilterMode.Point;
		m_depthTexture.Create();
	}


	void OnPostRender()
	{
		int size = depthSize();
		int w = size;
		int mipmap = 0;

		RenderTexture preDepthTextue = null;
		RenderTexture currentDepthTextue = null;

		while (w > 8) 
		{
			currentDepthTextue = RenderTexture.GetTemporary(w, w, 0, DepthTextureFormat);
			if (preDepthTextue == null) 
			{
				Graphics.Blit(Shader.GetGlobalTexture(CameraDepthTextureId), currentDepthTextue);
			}
			else
			{
				Graphics.Blit(preDepthTextue, currentDepthTextue, HizDepthTextureMat);
				RenderTexture.ReleaseTemporary(preDepthTextue);
			}
			Graphics.CopyTexture(currentDepthTextue,0,0, m_depthTexture,0, mipmap);
			preDepthTextue = currentDepthTextue;
			w /= 2;
			mipmap += 1;

		}
		if (currentDepthTextue != null) {
			RenderTexture.ReleaseTemporary(currentDepthTextue);
		}
	}

	void OnDestroy()
	{
		m_depthTexture?.Release();
		Destroy(m_depthTexture);
	}
}
