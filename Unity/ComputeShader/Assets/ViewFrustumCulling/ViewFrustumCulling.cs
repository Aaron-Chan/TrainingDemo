using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ViewFrustumCulling : MonoBehaviour
{
	public Material instanceMateiral;
	public ComputeShader cullingCS;
	public Mesh instanceMesh;
	public int instanceCount;
	public int subMeshIndex = 0;
	private int cachedInstanceCount = -1;
	private int cachedSubMeshIndex = -1;

	private ComputeBuffer positionBuffer;
	private ComputeBuffer drawPositionBuffer;
	ComputeBuffer mArgsBuffer;
	uint[] args = new uint[5] { 0, 0, 0, 0, 0 };
	int mKernelIndex;

	// Start is called before the first frame update
	void Start()
	{
		mArgsBuffer = new ComputeBuffer(1, args.Length * sizeof(uint), ComputeBufferType.IndirectArguments);
		mKernelIndex = cullingCS.FindKernel("ViewPortCulling");
		drawPositionBuffer = new ComputeBuffer(instanceCount, sizeof(float) * 16, ComputeBufferType.Append);

		UpdateBuffers();

	}

	void UpdateBuffers()
	{
		// Ensure submesh index is in range
		if (instanceMesh != null)
			subMeshIndex = Mathf.Clamp(subMeshIndex, 0, instanceMesh.subMeshCount - 1);

		// Positions
		if (positionBuffer != null)
			positionBuffer.Release();
		
		positionBuffer = new ComputeBuffer(instanceCount, 16 * sizeof(float));
		List<Matrix4x4> localToWorldMatrixs = new List<Matrix4x4>();
		for (int i = 0; i < instanceCount; i++)
		{
			float angle = Random.Range(0.0f, Mathf.PI * 2.0f);
			float distance = Random.Range(20.0f, 100.0f);
			float height = Random.Range(-2.0f, 2.0f);
			float size = Random.Range(0.05f, 0.25f);
			Vector4 position = new Vector4(Mathf.Sin(angle) * distance, height, Mathf.Cos(angle) * distance, size);
			localToWorldMatrixs.Add(Matrix4x4.TRS(position, Quaternion.identity, new Vector3(size, size, size)));
		}
		positionBuffer.SetData(localToWorldMatrixs);


		// Indirect args
		if (instanceMesh != null)
		{
			args[0] = (uint)instanceMesh.GetIndexCount(subMeshIndex);
			args[1] = (uint)instanceCount;
			args[2] = (uint)instanceMesh.GetIndexStart(subMeshIndex);
			args[3] = (uint)instanceMesh.GetBaseVertex(subMeshIndex);
		}
		else
		{
			args[0] = args[1] = args[2] = args[3] = 0;
		}
		mArgsBuffer.SetData(args);

		cachedInstanceCount = instanceCount;
		cachedSubMeshIndex = subMeshIndex;
	}

	void OnGUI()
	{
		GUI.Label(new Rect(265, 25, 200, 30), "Instance Count: " + instanceCount.ToString());
		instanceCount = (int)GUI.HorizontalSlider(new Rect(25, 20, 200, 30), (float)instanceCount, 1.0f, 5000000.0f);
	}

	// Update is called once per frame
	void Update()
	{
		if (cachedInstanceCount != instanceCount || cachedSubMeshIndex != subMeshIndex)
			UpdateBuffers();

		// Pad input
		if (Input.GetAxisRaw("Horizontal") != 0.0f)
			instanceCount = (int)Mathf.Clamp(instanceCount + Input.GetAxis("Horizontal") * 40000, 1.0f, 5000000.0f);


		//更新视锥体
		Camera camera = Camera.main;
		Vector4[] viewInfos = GetViewFrustumInfo();
		//Vector4[] viewInfos = CullTool.GetFrustumPlane(Camera.main);
		Vector4[] boxMaxMin = { instanceMesh.bounds.max , instanceMesh.bounds.min};

		drawPositionBuffer.SetCounterValue(0);
		cullingCS.SetVectorArray("planes", viewInfos);
		//cullingCS.SetVectorArray("boxMaxMin", boxMaxMin);
		cullingCS.SetBuffer(mKernelIndex, "input", positionBuffer);
		cullingCS.SetBuffer(mKernelIndex, "cullresult", drawPositionBuffer);
		cullingCS.SetInt("instanceCount", instanceCount);
		cullingCS.Dispatch(mKernelIndex, (instanceCount / 640)+1, 1, 1);


		ComputeBuffer.CopyCount(drawPositionBuffer,  mArgsBuffer,  sizeof(uint));
		instanceMateiral.SetBuffer("drawPositionBuffer", drawPositionBuffer);
		Graphics.DrawMeshInstancedIndirect(instanceMesh, subMeshIndex, instanceMateiral, new Bounds(Vector3.zero, new Vector3(200.0f, 200.0f, 200.0f)), mArgsBuffer);
	}

	Vector4 GetPlane(Vector3 normal, Vector3 point)
	{
		return new Vector4(normal.x, normal.y, normal.z, -Vector3.Dot(normal, point));
	}

	Vector4 GetPlane(Vector3 pointA, Vector3 pointB, Vector3 pointC)
	{
		//这里abc是顺时针的，自己想象一下就行了
		var normal = Vector3.Normalize(Vector3.Cross(pointB - pointA, pointC - pointA));
		return GetPlane(normal, pointA);
	}

	private Vector4[] GetViewFrustumInfo()
	{
		Vector4[] result = new Vector4[6];
		Camera camera = Camera.main;
		var cameraPos = camera.transform.position;
		var cameraDir = camera.transform.forward;
		var cameraTransform = camera.transform;
		var nearCenterPos = cameraPos + camera.transform.forward * camera.nearClipPlane;
		var farCenterPos = cameraPos + camera.transform.forward * camera.farClipPlane;
		var halfFov = camera.fieldOfView * 0.5f * Mathf.Deg2Rad;
		var halfFarHeight = Mathf.Tan(halfFov) * camera.farClipPlane;
		var halfFarWidth = halfFarHeight * camera.aspect;
		var right = cameraTransform.right * halfFarWidth;
		var up = cameraTransform.up * halfFarHeight;
		var leftBottom = farCenterPos - right - up;
		var leftTop = farCenterPos - right + up;
		var rightBottom = farCenterPos + right - up;
		var rightTop = farCenterPos + right + up;


		//unity是左手坐标系
		result[0] = GetPlane(cameraPos, leftBottom, leftTop);//left
		result[1] = GetPlane(cameraPos, leftTop, rightTop);//top
		result[2] = GetPlane(cameraPos, rightBottom, leftBottom);//bottom
		result[3] = GetPlane(cameraPos, rightTop, rightBottom);//right
		result[4] = GetPlane(-cameraDir, nearCenterPos);
		result[5] = GetPlane(cameraDir, farCenterPos);

		return result;
	}

	private void OnDestroy()
	{

		positionBuffer.Release();
		positionBuffer = null;

		drawPositionBuffer.Release();
		drawPositionBuffer = null;

		mArgsBuffer.Release();
		mArgsBuffer = null;
	}
}
