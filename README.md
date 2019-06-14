# Island Report
计图期末项目

| 姓名 | 学号 |
| ------ | ------ |
| 翦孟欣  | 16340092 |
| 黄悦    |  16340087 |
| 黎雨桐  |  16340110 |
| 吕雪萌   |  16340164 |
## 项目介绍
- 项目简介
	- 我们小组的项目是一款探索类的游戏。在本游戏中，玩家扮演的角色在一个神秘的岛屿上醒来。 玩家按照自己的步调探索这座神秘的岛屿。可以与环境进行互动，也可以只是单纯地欣赏景色，聆听声音。

- 开发环境：vs2017 + opengl
- 第三方库：glfw, glad, glm, assimp

## 功能列表

### 基础功能

- Camera Roaming
  - 参考learnopengl教程，实现视角随鼠标转动的效果。
- Simple Lighting
  - 我们的场景使用了平行光源。
- Texture Mapping
  - 我们对地面和正方体使用纹理映射，实现草坪和岩石的视觉效果。
- Shadow Mapping
  - 我们为正方体实现了阴影。
- Model import
  - 我们自己利用blender制作了小幽灵，并在网上查找精美的岛屿模型导入项目中。

### Bonus

- 天空盒的实现

  - 天空盒是一包含整个场景的立方体，通过给立方体内侧的六个表面贴图，让玩家产生巨大空间的错觉。

  - 天空盒的形式

    ![](https://learnopengl-cn.github.io/img/04/06/cubemaps_skybox.png)

  - 使用 loadCubemap 函数加载天空盒，参数为6个贴图的路径。

    ```c++
    unsigned int loadCubemap(vector<std::string> faces)
    {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
        int width, height, nrChannels;
        for (unsigned int i = 0; i < faces.size(); i++)
        {
            unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
            if (data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                             0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                );
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
        return textureID;
    }
    ```

  - 

  - 天空盒显示:

- 文字显示（尚未完成）

## 问题与解决方案
## 实现结果

## 成员分工
