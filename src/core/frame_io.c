#include "core/frame_io.h"

// -----===[ Globals ]===-----

frame_output *f_out = NULL;


// -----===[ Functions ]===-----

void set_frame_output(char *path, char *ext, frame_dump dump_method)
{
    char *path_cpy, *ext_cpy;

    path_cpy = malloc(strlen(path) + 1);

    if (path_cpy == NULL)
    {
        goto exit_fail;
    }

    strcpy(path_cpy, path);

    // Validate path
    char *path_file_sep = strrchr(path_cpy, '/');

    if (path_file_sep != NULL)
    {
        // Add a NULL terminator before the start of the filename
        *path_file_sep = '\0';
    }

    // Try to access the directory
    if (access(path_cpy, W_OK))
    {
        fprintf(stderr, "[ ERROR ] : Path for output '%s' does not exist or is not writable\n",
                path);

        exit(1);
    }

    if (path_file_sep != NULL)
    {
        // Reinsert the old '/'
        *path_file_sep = '/';
    }

    if (f_out == NULL)
    {
        f_out = malloc(sizeof(frame_output));

        if (f_out == NULL)
        {
            goto path_cleanup;
        }

        f_out->dump_method = dump_method;

        f_out->output_path = path_cpy;

        if (ext != NULL)
        {

            ext_cpy = malloc(strlen(ext) + 1);

            if (ext_cpy == NULL)
            {
                goto fout_cleanup;
            }

            strcpy(ext_cpy, ext);
            f_out->output_ext = ext_cpy;
        }
        else
        {
            f_out->output_ext = NULL;
        }
    }
    else
    {
        fputs("[ ERROR ] : Frame output config already set\n", stderr);

        exit(1);
    }

    return;

fout_cleanup:
    free(f_out);
    f_out = NULL;

path_cleanup:
    free(path_cpy);

exit_fail:
    fputs("[ ERROR ] : Not enough memory to create frame output config\n", stderr);

    exit(1);
}


void free_frame_output()
{
    if (f_out == NULL)
    {
        return;
    }

    free(f_out->output_path);

    if (f_out->output_ext != NULL)
    {
        free(f_out->output_ext);
    }

    free(f_out);
}


void save_frame(framebuf *fb, unsigned long framenum)
{
    if (f_out == NULL)
    {
        return;
    }

    // Construct the file name
    char *frame_name;

    size_t name_len = 0;

    // + 1 for the underscore
    name_len += strlen(f_out->output_path) + 1;

    if (f_out->output_ext != NULL)
    {
        // + 1 for the dot
        name_len += strlen(f_out->output_ext) + 1;
    }

    // Compute number of characters required to represent
    // the frame number
    int framechars = framenum;

    while (framechars > 0)
    {
        name_len += 1;
        framechars /= 10;
    }

    frame_name = malloc(name_len + 2);

    if (frame_name == NULL)
    {
        fputs("[ ERROR ] : Not enough memory to create frame output path\n", stderr);
        exit(1);
    }

    if (f_out->output_ext != NULL)
    {
        sprintf(frame_name, "%s_%lu.%s", f_out->output_path, framenum, f_out->output_ext);
    }
    else
    {
        sprintf(frame_name, "%s_%lu", f_out->output_path, framenum);
    }

    f_out->dump_method(frame_name, fb);

    free(frame_name);
}
