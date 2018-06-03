// LitesNes.Application.cpp : Defines the entry point for the console application.
//

#include "pch.h"
#include <iostream>
#include "windows.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "./GLFW/glfw3.h"

void setup(GLFWwindow* window);
void renderer(ImDrawData* draw_data);
void myImGui();
void populateIO(GLFWwindow* window);

int main()
{
	std::cout << "HAI" << std::endl;

	if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    static GLFWwindow* window {
        glfwCreateWindow(320, 240, "Simple example", NULL, NULL) 
    };

    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    // Close window on pressing ESC
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		mods;
		scancode;
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    });

    setup(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();

		//populateIO(window);
        // Use ImGui functions between here and Render()
        ImGui::NewFrame();

		/*
		char str[128] = "TestIn";
		float inFloat = 0.0f;
        // This creates a window
        ImGui::Begin("Title");

        ImGui::InputText("Hello, world!", str, IM_ARRAYSIZE(str));
        ImGui::InputFloat("Hello, world!", &inFloat, 0.1f,0.5f);
        ImGui::Text("Hello part 2");
        ImGui::Text("GOTCHA");
        ImGui::End();*/
		//ImGui::ShowDemoWindow();
		myImGui();

        // ImGui functions end here
        ImGui::Render();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);

}

/*! /brief Initialise the ImGuiIO struct.
 *
 *   The ImGuiIO struct is the main configuration and
 *   I/O between your application and ImGui.
 */
void setup(GLFWwindow* window)
{
    unsigned char* pixels;
    int width,
        height,
        display_width,
        display_height;
    GLuint g_FontTexture;
    ImGui::CreateContext();
    ImGuiIO& io { ImGui::GetIO() };

    io.Fonts->GetTexDataAsAlpha8(&pixels, &width, &height);

    // Upload texture to graphics system
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    glGenTextures(1, &g_FontTexture);
    glBindTexture(GL_TEXTURE_2D, g_FontTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);
    
    // Get display size
    glfwGetWindowSize(window, &width, &height);
    glfwGetFramebufferSize(window, &display_width, &display_height);

    io.DisplaySize = ImVec2((float)width, (float)height);
    io.RenderDrawListsFn = renderer;
    io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;

    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);
}

/*! /brief Boilerplate function for OpenGL 2.0 rendering.
 *
 *  This function isn't written by us, but is mandatory
 *  boilerplate from the library. It can be copy/pasted
 *  into your projects, but should really be part of the
 *  library itself?
 */
void renderer(ImDrawData* draw_data)
{
    ImGuiIO& io { ImGui::GetIO() };
    int fb_width { (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x) };
    int fb_height { (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y) };

    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnable(GL_TEXTURE_2D);

    // Setup viewport, orthographic projection matrix
    glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, io.DisplaySize.x, io.DisplaySize.y, 0.0f, -1.0f, +1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Render command lists
    #define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const unsigned char* vtx_buffer = (const unsigned char*)&cmd_list->VtxBuffer.front();
        const ImDrawIdx* idx_buffer = &cmd_list->IdxBuffer.front();
        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (void*)(vtx_buffer + OFFSETOF(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);
            }
            idx_buffer += pcmd->ElemCount;
        }
    }
    #undef OFFSETOF

    // Restore modified state
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindTexture(GL_TEXTURE_2D, last_texture);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
}

void myImGui()
{
	// Create a window called "My First Tool", with a menu bar.
	bool active = true;
ImGui::Begin("My First Tool", &active, ImGuiWindowFlags_MenuBar);
if (ImGui::BeginMenuBar())
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
        if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
        if (ImGui::MenuItem("Close", "Ctrl+W"))  { active = false; }
        ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
}

float color[4]{ 0.5f, 0.5f,0.5f,0.5f };
// Edit a color (stored as ~4 floats)
ImGui::ColorEdit4("Color", color);

// Plot some values
const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));
 
// Display contents in a scrolling region
ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
ImGui::BeginChild("Scrolling");
for (int n = 0; n < 50; n++)
    ImGui::Text("%04d: Some text", n);
ImGui::EndChild();
ImGui::End();
}

void populateIO(GLFWwindow* window)
{
    ImGuiIO& io { ImGui::GetIO() };

	double xPos;
	double yPos;
	glfwGetCursorPos(window, &xPos, &yPos);
	io.MousePos.x = (float)xPos;
	io.MousePos.y = (float)yPos;

	io.MouseDown[0] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	io.MouseDown[1] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	io.MouseDown[2] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);


	/*
    ImVec2      MousePos;                       // Mouse position, in pixels. Set to ImVec2(-FLT_MAX,-FLT_MAX) if mouse is unavailable (on another screen, etc.)
    bool        MouseDown[5];                   // Mouse buttons: left, right, middle + extras. ImGui itself mostly only uses left button (BeginPopupContext** are using right button). Others buttons allows us to track if the mouse is being used by your application + available to user as a convenience via IsMouse** API.
    float       MouseWheel;                     // Mouse wheel Vertical: 1 unit scrolls about 5 lines text.
    float       MouseWheelH;                    // Mouse wheel Horizontal. Most users don't have a mouse with an horizontal wheel, may not be filled by all back-ends.
    bool        MouseDrawCursor;                // Request ImGui to draw a mouse cursor for you (if you are on a platform without a mouse cursor).
    bool        KeyCtrl;                        // Keyboard modifier pressed: Control
    bool        KeyShift;                       // Keyboard modifier pressed: Shift
    bool        KeyAlt;                         // Keyboard modifier pressed: Alt
    bool        KeySuper;                       // Keyboard modifier pressed: Cmd/Super/Windows
    bool        KeysDown[512];                  // Keyboard keys that are pressed (ideally left in the "native" order your engine has access to keyboard keys, so you can use your own defines/enums for keys).
    ImWchar     InputCharacters[16+1];          // List of characters input (translated by user from keypress+keyboard state). Fill using AddInputCharacter() helper.
    float       NavInputs[ImGuiNavInput_COUNT]; // Gamepad inputs (keyboard keys will be auto-mapped and be written here by ImGui::NewFrame, all values will be cleared back to zero in ImGui::EndFrame)
	*/
}
