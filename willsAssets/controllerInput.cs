using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class controllerInput : MonoBehaviour
{
    public GameObject main_body;
    public GameObject cam;
    public GameObject headset;
    public float movement_speed = 2;
    public float angular_speed = 30;
    private Rigidbody rb;
    private Vector2 leftinput;
    private Vector2 rightinput;
    private float headset_x;
    private float headset_y;
    private int forward;
    private int right;
    private int up;
    private int rotation;
    private int panup;
    private int panright;

    public class InputPack
    {
        public int[] message { get; set; }
    }

    // Start is called before the first frame update
    void Start()
    {
        rb = main_body.GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        leftinput = OVRInput.Get(OVRInput.Axis2D.PrimaryThumbstick);
        rightinput = OVRInput.Get(OVRInput.Axis2D.SecondaryThumbstick);

        forward = (int)leftinput.y * 1000;
        right = (int)leftinput.x * 1000;
        up = (int)rightinput.y * 1000;
        rotation = (int)rightinput.x;
        panup = (int)headset.transform.localEulerAngles.y;
        panright = (int)headset.transform.localEulerAngles.x;

        headset_x = headset.transform.localEulerAngles.x;
        headset_y = headset.transform.localEulerAngles.y;



        cam.transform.localEulerAngles = new Vector3(headset_x, headset_y, 0);


        // forward backward
        main_body.transform.Translate(movement_speed * leftinput.x * Time.deltaTime, 0, 0, Space.Self);

        // up down
        main_body.transform.Translate(0, movement_speed * rightinput.y * Time.deltaTime, 0, Space.Self);


        // left right
        main_body.transform.Translate(0, 0, movement_speed * leftinput.y * Time.deltaTime, Space.Self);

        // rotate
        main_body.transform.Rotate(0,rightinput.x * Time.deltaTime * angular_speed,0);
        

    }
}
