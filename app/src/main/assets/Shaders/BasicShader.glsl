#shader vertex
attribute vec4 a_position;
void main()
{
    gl_Position = a_position;
}

#shader fragment
precision mediump float;
void main()
{
    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
