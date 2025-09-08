# CPU Fragment

## Use

Compile `libcpufrag.so` alongside files implementing the following;
- `fragment(tup3 *pos) -> tup3 col`
  - Can use uniforms `BACKBUF`, `FRAME_COUNT`, `CLOCK_NS` and `CONST_RAND`
    - Writing to these values is undefined
- `frag_init(int argc, char **argv)`
  - Must call `create_render_frame(unsigned int dimx, unsigned int dimy)`
  - Should call `set_frame_output(char *out_path, char *extension, frame_dump dump_fn)`
  - Can set `n_threads, n_jobs, n_frames`
- `frag_cleanup(void)`
