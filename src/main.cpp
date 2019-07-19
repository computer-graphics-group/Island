#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
//#include <learnopengl/model.h>
#include "model.h"

#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

#include "snow.h"
#include "TextRenderer.h"

#include <iostream>
#include "Mesh/myModel.h"
#include "WaveSea.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window,Model ourModel,Model island);
unsigned int loadTexture(const char *path);
void renderScene(const Shader &shader, Model ourModel);
void renderIsland(const Shader &shader, Model ourIsland);
void renderWood(const Shader &shader, Model ghost, Model wood);

void renderCube();
void renderQuad();
//skybox
unsigned int loadCubemap(vector<std::string> faces);
unsigned int control = 0;
// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

int _time = 0;
ParticleSystem *particleSystem = NULL;
//
// camera
Camera camera(glm::vec3(1.0f, 0.1f, 2.0));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;
int wood_num = 0;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool is_view_mode = false;
bool isSceneCollision = false;
bool isWoodCollision[3] = {false,false,false};
glm::vec3 ghostPos = glm::vec3(0.5f, -3.0f, 1.2);
glm::vec3 lastghostPos;
// meshes
unsigned int planeVAO;

// model frame
vector<Model> frames;

Camera_Movement ghost_move;

void loadAnimation(vector<Model>& frames, std::string filename, unsigned int start, unsigned int end)
{
	char tmp[200];
	for (int i = start; i <= end; i++)
	{
		if (i < 10)
			sprintf(tmp, "_00000%d", i);
		else if (i < 100)
			sprintf(tmp, "_0000%d", i);
		else if (i < 1000)
			sprintf(tmp, "_000%d", i);
		else if (i < 10000)
			sprintf(tmp, "_00%d", i);
		else if (i < 100000)
			sprintf(tmp, "_0%d", i);
		else if (i < 1000000)
			sprintf(tmp, "_%d", i);
		std::string tmp2(filename + tmp);
		tmp2 += ".obj";
		std::cout << tmp2 << std::endl;

		Model ourModel(tmp2);
		// unsigned int id = obj.load(tmp2);
		frames.push_back(ourModel);
	}
}




int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader shader("ShaderProgram/shadow_mapping.vs", "ShaderProgram/shadow_mapping.fs");
    Shader simpleDepthShader("ShaderProgram/shadow_mapping_depth.vs", "ShaderProgram/shadow_mapping_depth.fs");
    Shader debugDepthQuad("ShaderProgram/debug_quad.vs", "ShaderProgram/debug_quad_depth.fs");
    Shader textShader("ShaderProgram/text.vs", "ShaderProgram/text.fs");
    //skybox
    Shader skyboxShader("ShaderProgram/skyboxv.txt", "ShaderProgram/skyboxf.txt");
    TextRenderer textRender;
    //particlesystem
    //Shader particleShader("ShaderProgram/particlev.txt", "ShaderProgram/particlef.txt");
    Shader particleShader("ShaderProgram/particlev.txt", "ShaderProgram/particlef.txt");
    
	// 加载骨骼着色器
	Shader skinning("ShaderProgram/skinning.vs", "ShaderProgram/shader.fs");
	skinning.use();
	skinning.setInt("texture_diffuse1", 0);
	skinning.setInt("shadowMap", 1);

	// 加载带有动画的模型
	MyModel* am = new MyModel(glm::vec3(0.0f, -3.3f, 0.0f), -90.0f, glm::vec3(0.05, 0.05, 0.05), glm::vec3(1.0f, 0.0f, 0.0f));
	am->initAnimation("./resources/animation/deer1.md5mesh", skinning);

    //ParticleSystem
    particleSystem = new ParticleSystem(200, glm::vec3(0.0f, 0.0f, 0.0f), 20.0f, 20.0f, 5.0f);
	// load models
// -----------
	// load models
	loadAnimation(frames, "resources/objects/ghostAnima/ghost", 16, 29);

	Model island(FileSystem::getPath("resources/objects/islandModel/Island.obj"));
    Model wood(FileSystem::getPath("resources/objects/islandModel/wood.obj"));
    
    WaveSea sea;
    
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//skybox jpg

    vector<std::string> mid
    {
        "resources/textures/skybox/sea_ft1.jpg",
        "resources/textures/skybox/sea_bk1.jpg",
        "resources/textures/skybox/sea_up1.jpg",
        "resources/textures/skybox/sea_dn.jpg",
        "resources/textures/skybox/sea_rt1.jpg",
        "resources/textures/skybox/sea_lf1.jpg"
    };
    vector<std::string> night
    {
        "resources/textures/skybox/sea_ft2.jpg",
        "resources/textures/skybox/sea_bk2.jpg",
        "resources/textures/skybox/sea_up2.jpg",
        "resources/textures/skybox/sea_dn.jpg",
        "resources/textures/skybox/sea_rt2.jpg",
        "resources/textures/skybox/sea_lf2.jpg"
    };
    vector<std::string> faces
    {
        "resources/textures/skybox/sea_ft.jpg",
        "resources/textures/skybox/sea_bk.jpg",
        "resources/textures/skybox/sea_up.jpg",
        "resources/textures/skybox/sea_dn.jpg",
        "resources/textures/skybox/sea_rt.jpg",
        "resources/textures/skybox/sea_lf.jpg"
    };
	
	/*
	vector<std::string> faces
	{
		"../resources/textures/skybox/lake1_rt.jpg",
		"../resources/textures/skybox/lake1_lf.jpg",
		"../resources/textures/skybox/lake1_up.jpg",
		"../resources/textures/skybox/lake1_dn.jpg",
		"../resources/textures/skybox/lake1_ft.jpg",
		"../resources/textures/skybox/lake1_bk.jpg"
	};
	*/
    unsigned int cubemapTexture = loadCubemap(faces);
    unsigned int nightTexture = loadCubemap(night);
    unsigned int midTexture = loadCubemap(mid);
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
    };
    // plane VAO
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // load textures
    // -------------
    // unsigned int woodTexture = loadTexture(FileSystem::getPath("resources/textures/grassround.png").c_str());

    // configure depth map FBO
    // -----------------------
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    // shader configuration
    // --------------------
    shader.use();
    shader.setInt("diffuseTexture", 0);
    shader.setInt("shadowMap", 1);
    debugDepthQuad.use();
    debugDepthQuad.setInt("depthMap", 0);
	//skybox
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

    // lighting info
    // -------------
    glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
	float lpos[3] = { -2.0f, 4.0f, -1.0f };
//    bool is_run = true;
//    bool change = false;
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO();
//    (void)io;
//    ImGui_ImplGlfwGL3_Init(window, true);
//    ImGui::StyleColorsDark();
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window, frames[0],island);
//        ImGui_ImplGlfwGL3_NewFrame();
//        if (is_run)
//        {
//            ImGui::Begin("LightPos", &is_run);
//
//            ImGui::Text("Adjust parameters");
//
//            ImGui::SliderFloat3("Light Position", lpos, -30.0f, 30.0f);
//            ImGui::End();
//        }
        // change light position over time
        //lightPos.x = sin(glfwGetTime()) * 3.0f;
        //lightPos.z = cos(glfwGetTime()) * 2.0f;
        //lightPos.y = 5.0 + cos(glfwGetTime()) * 1.0f;

        // render
        // ------
        glClearColor(0.9f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		lightPos = glm::vec3(lpos[0], lpos[1], lpos[2]);
        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;
        //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        simpleDepthShader.use();
        simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);


        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            glActiveTexture(GL_TEXTURE0);
            //glBindTexture(GL_TEXTURE_2D, woodTexture);
			glCullFace(GL_FRONT);
			
		int index = (control / 2) % 14;
		renderScene(simpleDepthShader, frames[index]);
        renderIsland(simpleDepthShader, island);
        renderWood(simpleDepthShader, frames[0],wood);
		am->Render(simpleDepthShader);

			glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 2. render scene as normal using the generated depth/shadow map  
        // --------------------------------------------------------------
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        
//        ImGui::Begin("LightPos", &is_run);
//
//        ImGui::Text("Adjust parameters");
//
//        ImGui::SliderFloat("_fov", &camera.Zoom, -90.0f, 90.0f);
//        ImGui::End();
        
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        // set light uniforms
        shader.setVec3("viewPos", camera.Position);
        shader.setVec3("lightPos", lightPos);
        shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, woodTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
		
        control++;
		renderScene(shader, frames[index]);
        renderIsland(shader, island);
        renderWood(shader, frames[0],wood);

		skinning.use();
		skinning.setMat4("projection", projection);
		skinning.setMat4("view", view);
		// 设置光的参数
		skinning.setVec3("viewPos", camera.Position);
		skinning.setVec3("lightPos", lightPos);
		skinning.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, ResourceManager::GetTexture("glass"));
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		am->UpdateBone(glfwGetTime());
		am->Render(skinning);


        // render Depth map to quad for visual debugging
        // ---------------------------------------------
        debugDepthQuad.use();
        debugDepthQuad.setFloat("near_plane", near_plane);
        debugDepthQuad.setFloat("far_plane", far_plane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        //renderQuad();

        //particleSystem
		
        particleSystem->Update(deltaTime, camera.Position);
        //draw
        glm::mat4 pprojection(1.0f);
        glm::mat4 pview = camera.GetViewMatrix();
        pprojection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        particleShader.use();
        particleShader.setMat4("projection", pprojection);
        particleShader.setMat4("view", pview);
        particleSystem->Draw(particleShader);
        
        // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        if (_time < 1000) {
            //glBindTexture(GL_TEXTURE_CUBE_MAP, nightTexture);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            _time += 4;
        }
        else if (_time < 1500) {
            //glBindTexture(GL_TEXTURE_CUBE_MAP, nightTexture);
            glBindTexture(GL_TEXTURE_CUBE_MAP, midTexture);
            _time += 4;
        }
        else {
            glBindTexture(GL_TEXTURE_CUBE_MAP, nightTexture);
            _time += 4;
        }
        if (_time > 3000) {
            _time = 0;
        }
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
        
        
        //Œƒ◊÷‰÷»æ
        textRender.RenderWoodNum(textShader, wood_num);
        
        textRender.setSize(SCR_WIDTH, SCR_HEIGHT);
        if (wood_num == 3) {
            textRender.RenderText(textShader, "YOU ARE SAVED", SCR_WIDTH / 2 - 400, SCR_HEIGHT / 2, 2.0, glm::vec3(1.0f, 0.5f, 0.0f));
        }
        
        sea.Draw(projection, camera.GetViewMatrix(), camera.Position);
//        ImGui::Render();
//        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
//    ImGui_ImplGlfwGL3_Shutdown();
//    ImGui::DestroyContext();
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);

    glfwTerminate();
    return 0;
}

// renders the 3D scene
// --------------------

void renderIsland(const Shader &shader, Model ourIsland)
{
    //  floor
    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::scale(model, glm::vec3(0.5));
   // model = glm::translate(model, glm::vec3(0.0f, 1.0f, 4.0f));
    shader.setMat4("model", model);
    ourIsland.Draw(shader);
    
}
void renderWood(const Shader &shader, Model ghost, Model wood)
{
    //  floor
    glm::vec3 trans[3]={
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(8.0f, 0.0f, 10.0f),
        glm::vec3(2.0f, 0.0f, 15.0f)
    };
    for(int i=0;i<3;i++){
        if(isWoodCollision[i]){
            continue;
        }
        
        bool isCollision = wood.CheckAABBCollision(ghostPos+ghost.minPos*glm::vec3(0.14)+glm::vec3(0.0f,-1.0f,0.0f)-trans[i], ghostPos+ghost.maxPos*glm::vec3(0.14)-trans[i]);
        if(!isCollision){
            glm::mat4 model = glm::mat4(1.0f);
            //model = glm::scale(model, glm::vec3(0.5));
            model = glm::translate(model, trans[i]);
            shader.setMat4("model", model);
            wood.Draw(shader);
        }
        else{
            isWoodCollision[i] = true;
            wood_num++;
        }
    }

}
void renderScene(const Shader &shader, Model ourModel)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, ghostPos);
    model = glm::scale(model, glm::vec3(0.14));
    
    model = glm::rotate(model, (float)-ghost_move*90.0f+40.0f, glm::vec3(0.0, 1.0, 0.0));
    
    shader.setMat4("model", model);
    ourModel.Draw(shader);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window,Model ourModel,Model island)
{
	isSceneCollision=island.CheckPosCollision(ghostPos+ourModel.minPos*glm::vec3(0.14)+glm::vec3(0.0f,1.0f,0.0f), ghostPos+ourModel.maxPos*glm::vec3(0.14));
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if(is_view_mode){
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            camera.ProcessKeyboard(LEFT, deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }
    }
    else if(!isSceneCollision){
        lastghostPos = ghostPos;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
            ghostPos.z -= 2.5 * deltaTime;
            ghost_move = FORWARD;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            ghostPos.z += 2.5 * deltaTime;
            ghost_move = BACKWARD;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            ghostPos.x -= 2.5 * deltaTime;
            ghost_move = LEFT;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
            ghostPos.x += 2.5 * deltaTime;
            ghost_move = RIGHT;
        }
        camera.Position = ghostPos + glm::vec3(0.7f, 2.0f, 10.0f);
        camera.Front = -glm::vec3(0.7f, 0.0f, 10.0f);
    }else{
        ghostPos = lastghostPos;
    }
    
    
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        is_view_mode = !is_view_mode;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(is_view_mode){
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = 10*(xpos - lastX);
        float yoffset = 10 * (lastY - ypos); // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

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
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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
