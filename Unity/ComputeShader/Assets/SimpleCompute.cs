using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SimpleCompute : MonoBehaviour
{
	Image _image;
	public ComputeShader computeShader;


	// Start is called before the first frame update
	void Start()
    {
		InitShader();
    }

	private void InitShader()
	{
		_image = GetComponent<Image>();
		int _kernelIndex = computeShader.FindKernel("CSMain");
		int width = 1024, height = 1024;
		RenderTexture _rt = new RenderTexture(width, height, 0) { enableRandomWrite = true };
		_rt.Create();

		_image.material.SetTexture("_MainTex", _rt);
		computeShader.SetTexture(_kernelIndex, "Result", _rt);
		computeShader.Dispatch(_kernelIndex, width / 8, height / 8, 1);
	}

	// Update is called once per frame
	void Update()
    {
        
    }
}
