#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "linmath.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include "file_util.h"
#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

struct vertex3d
{
	float x,y,z;
	float nx,ny,nz;
	float r,g,b;
};

static vertex3d vertices3d[12];

static std::vector<tinyobj::shape_t> load_obj(const char* dir, const char* obj_name){
	//TODO separate into other file
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
  
	std::string err;
	
	std::string path = dir;
	//TODO check for / and .obj in dir and obj_name
	path += obj_name;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str(), dir);
  
	if (!err.empty()) { // `err` may contain warning message.
	  std::cerr << err << std::endl;
	}

	if (!ret) {
	  exit(1);
	}
	int i = 0;
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
	  // Loop over faces(polygon)
	  size_t index_offset = 0;
	  for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
	    int fv = shapes[s].mesh.num_face_vertices[f];
	
	    // Loop over vertices in the face.
	    for (size_t v = 0; v < fv; v++) {
	      // access to vertex
	      tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
	      tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
	      tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
	      tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
	      tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
	      tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
	      tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
	      
	      //TODO this gives a segfault with no textures, so check that they exist
	      //tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
	      //tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];
	      // Optional: vertex colors
	      // tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
	      // tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
	      // tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];
		printf("index:%i\n",idx);
	      	printf("vertex: \n x:%f y:%f z:%f\n",vx,vy,vz);
	      	printf("normal: \n x:%f y:%f z:%f\n",nx,ny,nz);
		vertices3d[i++]={vx,vy,vz,nx,ny,nz,((i%3==0)?1.f:0.f),((i%3==1)?1.f:0.f),((i%3==2)?1.f:0.f)};

	    }
	    index_offset += fv;
	
	    // per-face material
	    shapes[s].mesh.material_ids[f];
	  }

	}

}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
int main(void)
{
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location, vnorm_location;
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    
    //Load GL with glad
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);
    // NOTE: OpenGL error checks have been omitted for brevity
    // TODO add OpenGL error checks
    

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

    std::vector <tinyobj::shape_t> obj_shapes = load_obj("assets/","untitled.obj");

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3d), vertices3d, GL_STATIC_DRAW);
    

    const char* vertex_shader_text = read_ascii_2_char("./shaders/3dn.vert");
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
    

    const char* fragment_shader_text = read_ascii_2_char("./shaders/2d.frag");
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
    
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    
    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");
    vnorm_location = glGetAttribLocation(program, "vNorm");
    
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 9, (void*) 0);
    
    glEnableVertexAttribArray(vnorm_location);
    glVertexAttribPointer(vnorm_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 9, (void*) (sizeof(float) * 3));
    
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 9, (void*) (sizeof(float) * 6));
    
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        mat4x4 m, p, mvp;

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, (float) glfwGetTime());
        mat4x4_rotate_X(m, m, (float) glfwGetTime());

        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);
	//mvp = m;
	
        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
        glDrawArrays(GL_TRIANGLES, 0, 12);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

