xof 0302txt 0032
Header {1; 0; 1;}
 
// Created by 3DC
 
Material mat_0 {
    7.921569E-01;1.00E+00;9.921569E-01;7.490196E-01;;
    9.562E+01;
    1.00E+00;1.00E+00;1.00E+00;;
    1.584314E-01;2.00E-01;1.984314E-01;;
}
 
Frame Scene {
 
    FrameTransformMatrix {
        1.00E+00, 0.00E+00, 0.00E+00, 0.00E+00,
        0.00E+00, 1.00E+00, 0.00E+00, 0.00E+00,
        0.00E+00, 0.00E+00, 1.00E+00, 0.00E+00,
        0.00E+00, 0.00E+00, 0.00E+00, 1.00E+00;;
    }
 
    Frame THREE_dcreserved2 {
 
        FrameTransformMatrix {
            1.00E+00, 0.00E+00, 0.00E+00, 0.00E+00,
            0.00E+00, 1.00E+00, 0.00E+00, 0.00E+00,
            0.00E+00, 0.00E+00, 1.00E+00, 0.00E+00,
            5.00E+00, 1.00E+00, 0.00E+00, 1.00E+00;;
        }
    }
 
    Frame THREE_dcreserved6 {
 
        FrameTransformMatrix {
            1.00E+00, 0.00E+00, 0.00E+00, 0.00E+00,
            0.00E+00, 1.00E+00, 0.00E+00, 0.00E+00,
            0.00E+00, 0.00E+00, 1.00E+00, 0.00E+00,
            6.2775E+00, 3.0425E+00, 9.275E-01, 1.00E+00;;
        }
    }
 
    Frame Import_octahedron_X {
 
        FrameTransformMatrix {
            1.00E+00, 0.00E+00, 0.00E+00, 0.00E+00,
            0.00E+00, 1.00E+00, 0.00E+00, 0.00E+00,
            0.00E+00, 0.00E+00, 1.00E+00, 0.00E+00,
            0.00E+00, 0.00E+00, 0.00E+00, 1.00E+00;;
        }
 
        Frame Cone_001 {
 
            FrameTransformMatrix {
                1.00E+00, 0.00E+00, 0.00E+00, 0.00E+00,
                0.00E+00, 1.00E+00, 0.00E+00, 0.00E+00,
                0.00E+00, 0.00E+00, 1.00E+00, 0.00E+00,
                0.00E+00, 0.00E+00, 0.00E+00, 1.00E+00;;
            }
 
            Mesh Object {
                20;
                0.00E+00; 5.00E+00; 0.00E+00;,
                0.00E+00; 0.00E+00; 5.00E+00;,
                5.00E+00; 0.00E+00; 0.00E+00;,
                5.00E+00; 0.00E+00; 0.00E+00;,
                0.00E+00; 0.00E+00; 5.00E+00;,
                0.00E+00; -5.00E+00; 0.00E+00;,
                0.00E+00; -5.00E+00; 0.00E+00;,
                0.00E+00; 0.00E+00; 5.00E+00;,
                -5.00E+00; 0.00E+00; 0.00E+00;,
                0.00E+00; 5.00E+00; 0.00E+00;,
                0.00E+00; 5.00E+00; 0.00E+00;,
                0.00E+00; 0.00E+00; -5.00E+00;,
                -5.00E+00; 0.00E+00; 0.00E+00;,
                0.00E+00; 0.00E+00; -5.00E+00;,
                0.00E+00; -5.00E+00; 0.00E+00;,
                0.00E+00; 0.00E+00; -5.00E+00;,
                5.00E+00; 0.00E+00; 0.00E+00;,
                0.00E+00; 0.00E+00; -5.00E+00;,
                0.00E+00; 5.00E+00; 0.00E+00;,
                5.00E+00; 0.00E+00; 0.00E+00;;
                8;
                3;0,1,2;,
                3;3,4,5;,
                3;6,7,8;,
                3;8,7,9;,
                3;10,11,12;,
                3;8,13,6;,
                3;14,15,16;,
                3;17,18,19;;
                MeshMaterialList {
                    1;
                    1;
                    0;;
                    {mat_0}
                }
                MeshNormals {
                    8;
                    5.773503E-01; 5.773503E-01; 5.773503E-01;,
                    5.773503E-01; -5.773503E-01; 5.773503E-01;,
                    -5.773503E-01; -5.773503E-01; 5.773503E-01;,
                    -5.773503E-01; 5.773503E-01; 5.773503E-01;,
                    -5.773503E-01; 5.773503E-01; -5.773503E-01;,
                    -5.773503E-01; -5.773503E-01; -5.773503E-01;,
                    5.773503E-01; -5.773503E-01; -5.773503E-01;,
                    5.773503E-01; 5.773503E-01; -5.773503E-01;;
                    8;
                    3;0,0,0;,
                    3;1,1,1;,
                    3;2,2,2;,
                    3;3,3,3;,
                    3;4,4,4;,
                    3;5,5,5;,
                    3;6,6,6;,
                    3;7,7,7;;
                }
                MeshFaceWraps {
                    8;
                    0;; 0;;,
                    0;; 0;;,
                    0;; 0;;,
                    0;; 0;;,
                    0;; 0;;,
                    0;; 0;;,
                    0;; 0;;,
                    0;; 0;;;
                }
                MeshTextureCoords {
                    20;
                    6.79812E-01; 9.4337E-02;,
                    4.5355E-01; 3.20599E-01;,
                    6.79812E-01; 5.4686E-01;,
                    4.5314E-01; 5.4686E-01;,
                    4.5314E-01; 9.4337E-02;,
                    2.26878E-01; 3.20599E-01;,
                    4.52729E-01; 5.47271E-01;,
                    2.26467E-01; 7.73533E-01;,
                    4.52729E-01; 9.99795E-01;,
                    2.05E-04; 9.99795E-01;,
                    6.79402E-01; 9.99795E-01;,
                    4.5314E-01; 7.73533E-01;,
                    6.79402E-01; 5.47271E-01;,
                    2.26467E-01; 7.73533E-01;,
                    9.99795E-01; 9.99795E-01;,
                    9.99795E-01; 6.79812E-01;,
                    6.79812E-01; 9.99795E-01;,
                    2.26467E-01; 3.20599E-01;,
                    2.05E-04; 5.4686E-01;,
                    2.05E-04; 9.4337E-02;;
                }
            }
        }
    }
}
 
AnimationSet x3dc_0 {
 
 
    Animation THREE_dcreserved2 {
        {THREE_dcreserved2}
        AnimationOptions { 1; 0; }
        AnimationKey {
            2;
            1;
            0; 3; 5.00E+00, 1.00E+00, 0.00E+00;;;
        }
        AnimationKey {
            0;
            1;
            0; 4; 1.00E+00, 0.00E+00, 0.00E+00, 0.00E+00;;,
        }
    }
 
    Animation THREE_dcreserved6 {
        {THREE_dcreserved6}
        AnimationOptions { 1; 0; }
        AnimationKey {
            2;
            1;
            0; 3; 6.2775E+00, 3.0425E+00, 9.275E-01;;;
        }
        AnimationKey {
            0;
            1;
            0; 4; 1.00E+00, 0.00E+00, 0.00E+00, 0.00E+00;;,
        }
    }
 
    Animation Import_octahedron_X {
        {Import_octahedron_X}
        AnimationOptions { 1; 0; }
        AnimationKey {
            2;
            1;
            0; 3; 0.00E+00, 0.00E+00, 0.00E+00;;;
        }
        AnimationKey {
            0;
            1;
            0; 4; 1.00E+00, 0.00E+00, 0.00E+00, 0.00E+00;;,
        }
    }
 
    Animation Cone_001 {
        {Cone_001}
        AnimationOptions { 1; 0; }
        AnimationKey {
            2;
            1;
            0; 3; 0.00E+00, 0.00E+00, 0.00E+00;;;
        }
        AnimationKey {
            0;
            1;
            0; 4; 1.00E+00, 0.00E+00, 0.00E+00, 0.00E+00;;,
        }
    }
}
