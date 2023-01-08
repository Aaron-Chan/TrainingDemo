using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DrawParticle : MonoBehaviour
{
    // Start is called before the first frame update
    public ComputeShader UpdateParticleCS;
    public Material ParticleMaterial;
    float mTime;
    int mParticleCount;
    int mKernelIndex;
    ComputeBuffer mParticleBuffer;

    struct ParticleData
    {
        public  Vector3 position;
        public  Color color;
    };

    void Start()
    {
        mKernelIndex = UpdateParticleCS.FindKernel("UpdateParticle");
        mParticleCount = 20000;
        mParticleBuffer = new ComputeBuffer(mParticleCount, 3 * 4 + 4 * 4);
        ParticleData[] particleDatas = new ParticleData[mParticleCount];
        mParticleBuffer.SetData(particleDatas);
       

    }

    // Update is called once per frame
    void Update()
    {
        mTime += Time.deltaTime;
        UpdateParticle();
        DrawMyParicle();
    }

	private void DrawMyParicle()
	{
        ParticleMaterial.SetBuffer("_particleDataBuffer", mParticleBuffer);

    }

	private void UpdateParticle()
	{
        UpdateParticleCS.SetFloat("Time", Time.time);
        UpdateParticleCS.SetBuffer(mKernelIndex, "ParticleBuffer", mParticleBuffer);
        UpdateParticleCS.Dispatch(mKernelIndex, mParticleCount/1000, 1, 1);
    }

    void OnRenderObject()
    {
        ParticleMaterial.SetPass(0);
        Graphics.DrawProceduralNow(MeshTopology.Points, mParticleCount);
    }

	private void OnDestroy()
	{
        mParticleBuffer.Release();
        mParticleBuffer = null;
    }
}
