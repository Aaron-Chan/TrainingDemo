using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GrassGenerator : MonoBehaviour
{
    // Start is called before the first frame update
    public DepthTextureGenerator depthTextureGenerator;
    public ComputeShader cullingCS;
    public Mesh instanceMesh;
    public int subMeshIndex = 0;
    public int GrassCountPerRaw = 300;
    public Material instanceMateiral;


    int m_grassCount;
    ComputeBuffer grassMatrixBuffer;
    ComputeBuffer mArgsBuffer;
    ComputeBuffer drawPositionBuffer;
    uint[] args = new uint[5] { 0, 0, 0, 0, 0 };
    int mKernelIndex;
    int mCullresultId;
    int mHizDepthTextureId;
    int mVPId;

    void Start()
    {
        m_grassCount = GrassCountPerRaw * GrassCountPerRaw;
        InitComputeBuffer();
        InitGrassPosition();
        InitComputeShader();

    }

	private void InitComputeShader()
	{
        mKernelIndex = cullingCS.FindKernel("GrassCulling");
        cullingCS.SetBuffer(mKernelIndex, "input", grassMatrixBuffer);
        cullingCS.SetInt("instanceCount", m_grassCount);
        cullingCS.SetInt("depthTextureSize", depthTextureGenerator.depthSize());
        cullingCS.SetBool("isOpenGL", Camera.main.projectionMatrix.Equals(GL.GetGPUProjectionMatrix(Camera.main.projectionMatrix, false)));

        mCullresultId = Shader.PropertyToID("cullresult");
        mHizDepthTextureId = Shader.PropertyToID("hizTexture");
        mVPId = Shader.PropertyToID("vp");
    }


	private void InitComputeBuffer()
	{
        mArgsBuffer = new ComputeBuffer(1, args.Length * sizeof(uint), ComputeBufferType.IndirectArguments);
        drawPositionBuffer = new ComputeBuffer(m_grassCount, sizeof(float) * 16, ComputeBufferType.Append);
        grassMatrixBuffer = new ComputeBuffer(m_grassCount, sizeof(float) * 16, ComputeBufferType.Default);

        if (instanceMesh != null)
        {
            args[0] = (uint)instanceMesh.GetIndexCount(subMeshIndex);
            args[1] = (uint)m_grassCount;
            args[2] = (uint)instanceMesh.GetIndexStart(subMeshIndex);
            args[3] = (uint)instanceMesh.GetBaseVertex(subMeshIndex);
        }
        else
        {
            args[0] = args[1] = args[2] = args[3] = 0;
        }
        mArgsBuffer.SetData(args);
    }

	// Update is called once per frame
	void Update()
    {
        cullingCS.SetTexture(mKernelIndex, mHizDepthTextureId, depthTextureGenerator.depthTexture);
        cullingCS.SetMatrix(mVPId, GL.GetGPUProjectionMatrix(Camera.main.projectionMatrix, false) * Camera.main.worldToCameraMatrix);

        drawPositionBuffer.SetCounterValue(0);
        cullingCS.SetBuffer(mKernelIndex, mCullresultId, drawPositionBuffer);
        cullingCS.Dispatch(mKernelIndex, (m_grassCount / 640) + 1, 1, 1);


        ComputeBuffer.CopyCount(drawPositionBuffer, mArgsBuffer, sizeof(uint));
        instanceMateiral.SetBuffer("drawPositionBuffer", drawPositionBuffer);
        Graphics.DrawMeshInstancedIndirect(instanceMesh, subMeshIndex, instanceMateiral, new Bounds(Vector3.zero, new Vector3(100.0f, 100.0f, 100.0f)), mArgsBuffer);
    }

    //获取每个草的世界坐标矩阵
    void InitGrassPosition()
    {
        const int padding = 2;
        int width = (100 - padding * 2);
        int widthStart = -width / 2;
        float step = (float)width / GrassCountPerRaw;
        Matrix4x4[] grassMatrixs = new Matrix4x4[m_grassCount];
        for (int i = 0; i < GrassCountPerRaw; i++)
        {
            for (int j = 0; j < GrassCountPerRaw; j++)
            {
                Vector2 xz = new Vector2(widthStart + step * i, widthStart + step * j);
                Vector3 position = new Vector3(xz.x, GetGroundHeight(xz), xz.y);
                grassMatrixs[i * GrassCountPerRaw + j] = Matrix4x4.TRS(position, Quaternion.identity, Vector3.one);
            }
        }
        grassMatrixBuffer.SetData(grassMatrixs);
    }

    //通过Raycast计算草的高度
    float GetGroundHeight(Vector2 xz)
    {
        RaycastHit hit;
        if (Physics.Raycast(new Vector3(xz.x, 10, xz.y), Vector3.down, out hit, 20))
        {
            return 10 - hit.distance;
        }
        return 0;
    }

    private void OnDestroy()
    {

        grassMatrixBuffer.Release();
        grassMatrixBuffer = null;

        drawPositionBuffer.Release();
        drawPositionBuffer = null;

        mArgsBuffer.Release();
        mArgsBuffer = null;
    }
}
