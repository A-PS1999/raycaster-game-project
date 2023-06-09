#include "nuklear-sdl-gl.h"
#include "glad/glad.h"

#define NK_SHADER_VERSION "#version 300 es\n"

struct nk_sdl_device {
    struct nk_buffer cmds;
    struct nk_draw_null_texture null;
    GLuint vbo, vao, ebo;
    GLuint prog;
    GLuint vert_shdr;
    GLuint frag_shdr;
    GLint attrib_pos;
    GLint attrib_uv;
    GLint attrib_col;
    GLint uniform_tex;
    GLint uniform_proj;
    GLuint font_tex;
};

struct nk_sdl_vertex {
    float position[2];
    float uv[2];
    nk_byte col[4];
};

static struct nk_sdl {
    SDL_Window* win;
    struct nk_sdl_device ogl;
    struct nk_context ctx;
    struct nk_font_atlas atlas;
} sdl;

NK_API void nk_sdl_device_create(void) {
    GLint status;

    static const GLchar* vertex_shader =
        NK_SHADER_VERSION
        "uniform mat4 ProjMtx;\n"
        "in vec2 Position;\n"
        "in vec2 TexCoord;\n"
        "in vec4 Color;\n"
        "out vec2 Frag_UV;\n"
        "out vec4 Frag_Color;\n"
        "void main() {\n"
        "   Frag_UV = TexCoord;\n"
        "   Frag_Color = Color;\n"
        "   gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
        "}\n";

    static const GLchar* fragment_shader =
        NK_SHADER_VERSION
        "precision mediump float;\n"
        "uniform sampler2D Texture;\n"
        "in vec2 Frag_UV;\n"
        "in vec4 Frag_Color;\n"
        "out vec4 Out_Color;\n"
        "void main(){\n"
        "   Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
        "}\n";

    struct nk_sdl_device* device = &sdl.ogl;
    nk_buffer_init_default(&device->cmds);
    device->prog = glCreateProgram();
    device->vert_shdr = glCreateShader(GL_VERTEX_SHADER);
    device->frag_shdr = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(device->vert_shdr, 1, &vertex_shader, 0);
    glShaderSource(device->frag_shdr, 1, &fragment_shader, 0);
    glCompileShader(device->vert_shdr);
    glCompileShader(device->frag_shdr);
    glGetShaderiv(device->vert_shdr, GL_COMPILE_STATUS, &status);
    assert(status == GL_TRUE);
    glGetShaderiv(device->frag_shdr, GL_COMPILE_STATUS, &status);
    assert(status == GL_TRUE);
    glAttachShader(device->prog, device->vert_shdr);
    glAttachShader(device->prog, device->frag_shdr);
    glLinkProgram(device->prog);
    glGetProgramiv(device->prog, GL_LINK_STATUS, &status);
    assert(status == GL_TRUE);

    device->uniform_tex = glGetUniformLocation(device->prog, "Texture");
    device->uniform_proj = glGetUniformLocation(device->prog, "ProjMtx");
    device->attrib_pos = glGetAttribLocation(device->prog, "Position");
    device->attrib_uv = glGetAttribLocation(device->prog, "TexCoord");
    device->attrib_col = glGetAttribLocation(device->prog, "Color");

    {
        GLsizei vs = sizeof(struct nk_sdl_vertex);
        size_t vp = offsetof(struct nk_sdl_vertex, position);
        size_t vt = offsetof(struct nk_sdl_vertex, uv);
        size_t vc = offsetof(struct nk_sdl_vertex, col);

        glGenBuffers(1, &device->vbo);
        glGenBuffers(1, &device->ebo);
        glGenVertexArrays(1, &device->vao);

        glBindVertexArray(device->vao);
        glBindBuffer(GL_ARRAY_BUFFER, device->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, device->ebo);

        glEnableVertexAttribArray((GLuint)device->attrib_pos);
        glEnableVertexAttribArray((GLuint)device->attrib_uv);
        glEnableVertexAttribArray((GLuint)device->attrib_col);

        glVertexAttribPointer((GLuint)device->attrib_pos, 2, GL_FLOAT, GL_FALSE, vs, (void*)vp);
        glVertexAttribPointer((GLuint)device->attrib_uv, 2, GL_FLOAT, GL_FALSE, vs, (void*)vt);
        glVertexAttribPointer((GLuint)device->attrib_col, 4, GL_UNSIGNED_BYTE, GL_TRUE, vs, (void*)vc);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
};

NK_INTERN void nk_sdl_device_upload_atlas(const void* image, int width, int height) {
    struct nk_sdl_device* device = &sdl.ogl;
    glGenTextures(1, &device->font_tex);
    glBindTexture(GL_TEXTURE_2D, device->font_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, image);
};

NK_API void nk_sdl_device_destroy(void) {
    struct nk_sdl_device* device = &sdl.ogl;
    glDetachShader(device->prog, device->vert_shdr);
    glDetachShader(device->prog, device->frag_shdr);
    glDeleteShader(device->vert_shdr);
    glDeleteShader(device->frag_shdr);
    glDeleteProgram(device->prog);
    glDeleteTextures(1, &device->font_tex);
    glDeleteBuffers(1, &device->vbo);
    glDeleteBuffers(1, &device->ebo);
    nk_buffer_free(&device->cmds);
};

NK_API void nk_sdl_render(enum nk_anti_aliasing AA, int max_vertex_buffer, int max_element_buffer) {
    struct nk_sdl_device* device = &sdl.ogl;
    int width, height;
    int display_width, display_height;
    struct nk_vec2 scale;
    GLfloat ortho[4][4] = {
        {2.0f, 0.0f, 0.0f, 0.0f},
        {0.0f,-2.0f, 0.0f, 0.0f},
        {0.0f, 0.0f,-1.0f, 0.0f},
        {-1.0f,1.0f, 0.0f, 1.0f},
    };
    SDL_GetWindowSize(sdl.win, &width, &height);
    SDL_GL_GetDrawableSize(sdl.win, &display_width, &display_height);
    ortho[0][0] /= (GLfloat)width;
    ortho[1][1] /= (GLfloat)height;

    scale.x = (float)display_width / (float)width;
    scale.y = (float)display_height / (float)height;

    /* setup global state */
    glViewport(0, 0, display_width, display_height);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glActiveTexture(GL_TEXTURE0);

    /* setup program */
    glUseProgram(device->prog);
    glUniform1i(device->uniform_tex, 0);
    glUniformMatrix4fv(device->uniform_proj, 1, GL_FALSE, &ortho[0][0]);
    {
        /* convert from command queue into draw list and draw to screen */
        const struct nk_draw_command* cmd;
        void* vertices, * elements;
        const nk_draw_index* offset = NULL;
        struct nk_buffer vbuf, ebuf;

        /* allocate vertex and element buffer */
        glBindVertexArray(device->vao);
        glBindBuffer(GL_ARRAY_BUFFER, device->vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, device->ebo);

        glBufferData(GL_ARRAY_BUFFER, max_vertex_buffer, NULL, GL_STREAM_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_element_buffer, NULL, GL_STREAM_DRAW);

        /* load vertices/elements directly into vertex/element buffer */
        vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        elements = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
        {
            /* fill convert configuration */
            struct nk_convert_config config;
            static const struct nk_draw_vertex_layout_element vertex_layout[] = {
                {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_sdl_vertex, position)},
                {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_sdl_vertex, uv)},
                {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(struct nk_sdl_vertex, col)},
                {NK_VERTEX_LAYOUT_END}
            };
            NK_MEMSET(&config, 0, sizeof(config));
            config.vertex_layout = vertex_layout;
            config.vertex_size = sizeof(struct nk_sdl_vertex);
            config.vertex_alignment = NK_ALIGNOF(struct nk_sdl_vertex);
            config.tex_null = device->null;
            config.circle_segment_count = 22;
            config.curve_segment_count = 22;
            config.arc_segment_count = 22;
            config.global_alpha = 1.0f;
            config.shape_AA = AA;
            config.line_AA = AA;

            /* setup buffers to load vertices and elements */
            nk_buffer_init_fixed(&vbuf, vertices, (nk_size)max_vertex_buffer);
            nk_buffer_init_fixed(&ebuf, elements, (nk_size)max_element_buffer);
            nk_convert(&sdl.ctx, &device->cmds, &vbuf, &ebuf, &config);
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

        /* iterate over and execute each draw command */
        nk_draw_foreach(cmd, &sdl.ctx, &device->cmds) {
            if (!cmd->elem_count) continue;
            glBindTexture(GL_TEXTURE_2D, (GLuint)cmd->texture.id);
            glScissor((GLint)(cmd->clip_rect.x * scale.x),
                (GLint)((height - (GLint)(cmd->clip_rect.y + cmd->clip_rect.h)) * scale.y),
                (GLint)(cmd->clip_rect.w * scale.x),
                (GLint)(cmd->clip_rect.h * scale.y));
            glDrawElements(GL_TRIANGLES, (GLsizei)cmd->elem_count, GL_UNSIGNED_SHORT, offset);
            offset += cmd->elem_count;
        }
        nk_clear(&sdl.ctx);
        nk_buffer_clear(&device->cmds);
    }

    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glDisable(GL_SCISSOR_TEST);
};

static void nk_sdl_clipboard_paste(nk_handle usr, struct nk_text_edit* edit) {
    const char* text = SDL_GetClipboardText();
    if (text) nk_textedit_paste(edit, text, nk_strlen(text));
    (void)usr;
};

static void nk_sdl_clipboard_copy(nk_handle usr, const char* text, int len) {
    char* str = 0;
    (void)usr;
    if (!len) return;
    str = (char*)malloc((size_t)len + 1);
    if (!str) return;
    memcpy(str, text, (size_t)len);
    str[len] = '\0';
    SDL_SetClipboardText(str);
    free(str);
};

NK_API struct nk_context* nk_sdl_init(SDL_Window* window) {
    sdl.win = window;
    nk_init_default(&sdl.ctx, 0);
    sdl.ctx.clip.copy = nk_sdl_clipboard_copy;
    sdl.ctx.clip.paste = nk_sdl_clipboard_paste;
    sdl.ctx.clip.userdata = nk_handle_ptr(0);
    nk_sdl_device_create();
    return &sdl.ctx;
};

NK_API void nk_sdl_font_stash_begin(struct nk_font_atlas** atlas)
{
    nk_font_atlas_init_default(&sdl.atlas);
    nk_font_atlas_begin(&sdl.atlas);
    *atlas = &sdl.atlas;
};

NK_API void nk_sdl_font_stash_end(void)
{
    const void* image; int w, h;
    image = nk_font_atlas_bake(&sdl.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    nk_sdl_device_upload_atlas(image, w, h);
    nk_font_atlas_end(&sdl.atlas, nk_handle_id((int)sdl.ogl.font_tex), &sdl.ogl.null);
    if (sdl.atlas.default_font)
        nk_style_set_font(&sdl.ctx, &sdl.atlas.default_font->handle);
};

NK_API int nk_sdl_handle_event(SDL_Event* e) {
    struct nk_context* ctx = &sdl.ctx;
    if (e->type == SDL_KEYUP || e->type == SDL_KEYDOWN) {
        /* key events */
        int down = e->type == SDL_KEYDOWN;
        const Uint8* state = SDL_GetKeyboardState(0);
        SDL_Keycode sym = e->key.keysym.sym;
        if (sym == SDLK_RSHIFT || sym == SDLK_LSHIFT)
            nk_input_key(ctx, NK_KEY_SHIFT, down);
        else if (sym == SDLK_DELETE)
            nk_input_key(ctx, NK_KEY_DEL, down);
        else if (sym == SDLK_RETURN)
            nk_input_key(ctx, NK_KEY_ENTER, down);
        else if (sym == SDLK_TAB)
            nk_input_key(ctx, NK_KEY_TAB, down);
        else if (sym == SDLK_BACKSPACE)
            nk_input_key(ctx, NK_KEY_BACKSPACE, down);
        else if (sym == SDLK_HOME) {
            nk_input_key(ctx, NK_KEY_TEXT_START, down);
            nk_input_key(ctx, NK_KEY_SCROLL_START, down);
        }
        else if (sym == SDLK_END) {
            nk_input_key(ctx, NK_KEY_TEXT_END, down);
            nk_input_key(ctx, NK_KEY_SCROLL_END, down);
        }
        else if (sym == SDLK_PAGEDOWN) {
            nk_input_key(ctx, NK_KEY_SCROLL_DOWN, down);
        }
        else if (sym == SDLK_PAGEUP) {
            nk_input_key(ctx, NK_KEY_SCROLL_UP, down);
        }
        else if (sym == SDLK_z)
            nk_input_key(ctx, NK_KEY_TEXT_UNDO, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_r)
            nk_input_key(ctx, NK_KEY_TEXT_REDO, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_c)
            nk_input_key(ctx, NK_KEY_COPY, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_v)
            nk_input_key(ctx, NK_KEY_PASTE, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_x)
            nk_input_key(ctx, NK_KEY_CUT, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_b)
            nk_input_key(ctx, NK_KEY_TEXT_LINE_START, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_e)
            nk_input_key(ctx, NK_KEY_TEXT_LINE_END, down && state[SDL_SCANCODE_LCTRL]);
        else if (sym == SDLK_UP)
            nk_input_key(ctx, NK_KEY_UP, down);
        else if (sym == SDLK_DOWN)
            nk_input_key(ctx, NK_KEY_DOWN, down);
        else if (sym == SDLK_LEFT) {
            if (state[SDL_SCANCODE_LCTRL])
                nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, down);
            else nk_input_key(ctx, NK_KEY_LEFT, down);
        }
        else if (sym == SDLK_RIGHT) {
            if (state[SDL_SCANCODE_LCTRL])
                nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, down);
            else nk_input_key(ctx, NK_KEY_RIGHT, down);
        }
        else return 0;
        return 1;
    }
    else if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
        /* mouse button */
        int down = e->type == SDL_MOUSEBUTTONDOWN;
        const int x = e->button.x, y = e->button.y;
        if (e->button.button == SDL_BUTTON_LEFT) {
            if (e->button.clicks > 1)
                nk_input_button(ctx, NK_BUTTON_DOUBLE, x, y, down);
            nk_input_button(ctx, NK_BUTTON_LEFT, x, y, down);
        }
        else if (e->button.button == SDL_BUTTON_MIDDLE)
            nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, down);
        else if (e->button.button == SDL_BUTTON_RIGHT)
            nk_input_button(ctx, NK_BUTTON_RIGHT, x, y, down);
        return 1;
    }
    else if (e->type == SDL_MOUSEMOTION) {
        /* mouse motion */
        if (ctx->input.mouse.grabbed) {
            int x = (int)ctx->input.mouse.prev.x, y = (int)ctx->input.mouse.prev.y;
            nk_input_motion(ctx, x + e->motion.xrel, y + e->motion.yrel);
        }
        else nk_input_motion(ctx, e->motion.x, e->motion.y);
        return 1;
    }
    else if (e->type == SDL_TEXTINPUT) {
        /* text input */
        nk_glyph glyph;
        memcpy(glyph, e->text.text, NK_UTF_SIZE);
        nk_input_glyph(ctx, glyph);
        return 1;
    }
    else if (e->type == SDL_MOUSEWHEEL) {
        /* mouse wheel */
        nk_input_scroll(ctx, nk_vec2((float)e->wheel.x, (float)e->wheel.y));
        return 1;
    }
    return 0;
};

NK_API void nk_sdl_shutdown(void) {
    nk_font_atlas_clear(&sdl.atlas);
    nk_free(&sdl.ctx);
    nk_sdl_device_destroy();
    memset(&sdl, 0, sizeof(sdl));
};