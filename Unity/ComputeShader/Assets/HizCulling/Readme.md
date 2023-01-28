1. 先进行视椎剔除
    
    使用drawinstanceIndirect

    由于草的较小一般不会出现横穿镜头的情况，所以在clip space判断是否在clip space空间内，注意区分directX和opengl的范围
    
2. Hiz遮挡剔除
    
    2.1 如何生成Hiz图

    1. 可以借preZ的深度图去生成Hiz图
    2. 可以使用shader或者cs去生成， UE是使用cs去生成
    3. 需要考虑深度图没2的幂的情况，需要先将深度图拷贝采样到2的幂大小（取屏幕分辨率较长的边的下一个2的幂次方值作为RenderTexture的大小），方便后续进行downsample，采样的方式需要是point。
    4. 利用Graphics.Blit生成mipmap，配合DepthTextureMipmapCalculator shader，C#中利用这个Shader生成我们的Mipmap了 _CameraDepthTexture是第一个
    5. 在生成mipmap的shader中current的size是w，那么pre的size是2w，current对应像素就等于2w+1，2w|2w,2w|2w,2w+1|2w+1,2w+1总共4个点的最远值
    4. 对比深度的大小需要考虑reserve z的情况
    5. downsample是采样4个像素成1个像素，是取4个像素中离相机最远的，假如最远的像素都能挡着物体，那么这个物体肯定被挡着了
    
    2.2 如何对比
    1. 对于物体取一个代表的depth跟HizTexture进行对比
    2. 去使用物体的aabb，然后在ndc坐标下求8个点的3个轴最小点和最大点
    3. 先进行视椎体剔除
    4. cs需要什么
        1. 变量：草的个数，相机的vp，草的包围盒（模型空间），草的世界矩阵，得出的裁剪的矩阵，hizTexture
        2. 逻辑：现将包围盒转换到ndc坐标，然后先进行视椎体剔除，接着取3个轴的最小和最大值，取其中作为草的深度值
        3. 计算max-min(XY)的最大值作为，计算出对应的mipmap再得到对比的像素，进行对比即可

    

