using System;
using System.Collections; 
using System.Collections.Generic; 
using System.Net; 
using System.Net.Sockets; 
using System.Text; 
using System.Threading; 
using UnityEngine;  
using MsgPack.Serialization;
using System.Threading.Tasks;
using System.Windows;
using System.IO;

public class PhotoEntry
{
	public int[] Image { get; set; }
}

public class client : MonoBehaviour
{
	public Thread tcpListenerThread;
	public String host = "192.168.1.3";
	public Int32 port = 40810;

	internal Boolean socket_ready = false;
	internal String input_buffer = "";
	TcpClient tcp_socket;
	NetworkStream net_stream;

	StreamWriter socket_writer;
	StreamReader socket_reader;

	public GameObject headset;
	private Vector2 leftinput;
	private Vector2 rightinput;
	private int forward;
	private int right;
	private int up;
	private int rotation;
	private int panup;
	private int panright;

	// Use this for initialization
	void Start()
	{
		// Start TcpServer background thread 		
		//ListenForIncommingRequests();
		tcpListenerThread = new Thread(new ThreadStart(ListenForIncommingRequests));
		tcpListenerThread.IsBackground = true;
		tcpListenerThread.Start();
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
		// if (Input.GetKeyDown(KeyCode.Space)) {             
		// 	SendMessage();         
		// } 	
	}

	public void setupSocket()
	{
		try
		{
			tcp_socket = new TcpClient(host, port);
			net_stream = tcp_socket.GetStream();
			socket_writer = new StreamWriter(net_stream);
			socket_reader = new StreamReader(net_stream);
			socket_ready = true;
			Debug.Log("connected");
		}
		catch (Exception e)
		{
			Debug.Log("Socket error: " + e); // Something went wrong
		}
	}

	public void closeSocket()
	{
		if (!socket_ready)
			//Debug.Log("(closeSocket) socket not ready");
			return;

		socket_writer.Close();
		socket_reader.Close();
		tcp_socket.Close();
		socket_ready = false;
		Debug.Log("connection closed");
	}

	public void writeSocket(byte[] data)
	{
		for (int i = 0; i < data.Length; i++)
		{
			net_stream.WriteByte(data[i]);
		}
		Debug.Log("Sent: " + string.Join(", ", data));
	}

	public byte[] readSocket()
	{
		byte[] bytes = new byte[100];
		int c;
		for (int i = 0; i < 100; i++)
		{
			c = net_stream.ReadByte();
			bytes[i] = (byte)c;
			if (i > 4 && bytes[i] == 0x41 && bytes[i - 1] == 0x38 && bytes[i - 2] == 0x30 && bytes[i - 3] == 0x34)
			{
				break;
			}
		}

		Debug.Log("received: " + string.Join(", ", bytes));
		return bytes;
	}

	void ListenForIncommingRequests()
	{
		try
		{
			Debug.Log("connecting...");
			setupSocket();
			... (27 lines left)
Collapse
message.txt
4 KB
here is what I narrowed it down to so far. Very basic and doesn't do message pack yet. I'm pretending to do msgpack by adding the bytes manually lmao
oh I figured out a way to fix the overcurrent thing on ESP
I just put 5V into Vin
codydeyarmin — 12 / 01 / 2022 2:59 AM
	Oh?
Gary — 12 / 01 / 2022 2:59 AM
	don't know why I didn't think about that a while ago
	codydeyarmin — 12 / 01 / 2022 2:59 AM
		The voltage regulator I have is mainly for the stm
		I didnt think about the esp
		but tbh if it takes 5v it can probably piggy back off the camera
		Gary — 12 / 01 / 2022 3:00 AM
			well the L4 board itself has a 5V out
codydeyarmin — 12 / 01 / 2022 3:00 AM
	i doubt the camera and esp will take more than an amp and I have a 5v non-adjustable regulator.
or that
but that may run into the same current issue as the arduino? no
idk
we still have time to figure it out
worse case we just grab another 5v regulator
they're like 2$
But I'm pretty sure the stm boards need 3.3v 
so I got that covered now at least
Gary — 12 / 01 / 2022 3:02 AM
	initially, I was trying to wire the 3.3V from the L4 board directly into Vin on the ESP32, but I was reading eh voltage to be like 2.9, which means the ESP32 wouldn't work properly. So I used the 3.3V pin on ESP instead of Vin for power since it was providing the correct voltage, but that may not have been regulated
then I figured I could just plug 5V into Vin since it would regulate it to what the ESP needs and now everything is great
codydeyarmin — 12 / 01 / 2022 3:03 AM
	nice
how fast can you pass data
Gary — 12 / 01 / 2022 3:03 AM
	I'll have to check that metric again soon
codydeyarmin — 12 / 01 / 2022 3:03 AM
	I would like to see like 50hz but imma be happy at like 10hz lol
no worries
Gary — 12 / 01 / 2022 3:03 AM
	I want to mess with the baud rates more
codydeyarmin — 12 / 01 / 2022 3:03 AM
	like i mentioned the flight loop is gonna do a data validity check which is basically just gonna use a systick ms count to figure when the last data received was and discard if its too old
Gary — 12 / 01 / 2022 3:04 AM
	okay sounds good
codydeyarmin — 12 / 01 / 2022 3:04 AM
	that way the drone doesn't get stuck flying forward or somethin
Gary — 12 / 01 / 2022 9:26 AM
	Gonna be about 10 min late this morning
	codydeyarmin — 12 / 01 / 2022 9:26 AM
		hahah imma be about 20
Triple XZ — 12 / 01 / 2022 9:58 AM
	yeah uhmm i just woke up lol
soo uh no msgpack in unity lol?
Gary — 12 / 01 / 2022 10:04 AM
	I'm gonna try to get that working now
just data types in c# are awful
Gary — 12 / 01 / 2022 10:40 AM
	@codydeyarmin apparently there are 3 SPIs on the L4 so I may try to use one of them
	Oh also I may try to use the ESP as an access point as well as the server( if I can) which might help with speed / latency
	I would think that would only help with latency, but maybe the speed could improve too
	Gary — 12 / 01 / 2022 3:01 PM
		Did y’all want to meet up today? Or should we try some time this weekend?
		deliverygaming — 12 / 01 / 2022 3:03 PM
			I’m around if we wanna meet, not sure my availability this weekend tho
		Tomorrow also works just have sabbath then work on Sunday night ??
codydeyarmin — 12 / 01 / 2022 5:22 PM
	Does midday sunday work for yall.
	I can host
	deliverygaming — 12 / 01 / 2022 5:23 PM
	Works for me
	Gary — 12 / 01 / 2022 5:24 PM
	sounds good to me
	codydeyarmin — 12 / 01 / 2022 5:24 PM
	Cool!
	Gary — 12 / 01 / 2022 5:25 PM
	we thinking like 1pm ?
	deliverygaming — 12 / 01 / 2022 5:27 PM
	Works for me just have to be at my job at 3pm
	codydeyarmin — 12 / 01 / 2022 5:34 PM
	Im up for whatever i can do anytime.
	Gary — 12 / 02 / 2022 11:42 AM
		Me finally realizing why the ESP32 has like 30 pins - it's for all them comm protocols 
		TheTribalTurtle — 12 / 02 / 2022 3:47 PM
			I have gotten inputs on oculus working
		But for some reason I couldnt get our cloud project to run so I made a new project to test them
		So I can just add the scripts to our project
		Gary — 12 / 02 / 2022 3:56 PM
		Did my code get automatically uploaded or something ? I had to regen code or something because I was using a different Unity version
		So not sure if that’s the issue or something else
				Triple XZ — 12 / 02 / 2022 3:56 PM
					nah you should be doing it manually
		unless your version does so my default
		TheTribalTurtle — 12 / 02 / 2022 4:13 PM
			Yeah its manual
		Gary — 12 / 02 / 2022 4:13 PM
			okay sweet
			I kinda don't want to push just cause I don't want my Unity version to mess anything up or anything
			Once I'm finished with the client code I can put it on Github and yall can snag it to put on the cloud thingy
		I am so fucking lucky lmaoooooo. That one header I soldered onto the L4A6 board is so perfectly placed that it automatically had the default SPI1 pins included ??
Gary — 12 / 02 / 2022 4:59 PM
		what is the model of camera we are using?
		codydeyarmin — Yesterday at 12:04 PM
		Wyze cam v2
		codydeyarmin — Yesterday at 12:55 PM
		I'm working at getting the board talking over i2c to the accelerometer. Then progressing to getting the dshot edited for this board from https://github.com/mokhwasomssi/stm32_hal_dshot
		GitHub
		GitHub - mokhwasomssi / stm32_hal_dshot: Dshot protocol library with ...
Dshot protocol library with STM32 HAL driver.Contribute to mokhwasomssi / stm32_hal_dshot development by creating an account on GitHub.
 GitHub - mokhwasomssi / stm32_hal_dshot: Dshot protocol library with ...
I think dshot will just require reconfiguring the timers used and the calls made.
idk, gonna knock out the accelerometer as that should be very easy. Its reading two sets of 3 registers.
		@Gary are you currently using the lpuart1 as a console port. The tutorials I've been working off use it as the console and its seems to be setup by default.
		Gary — Yesterday at 1:14 PM
		I was using USART1.I’ve almost got SPI1 working
	   codydeyarmin — Yesterday at 1:14 PM
	   ok
		I ask Cause I'm gonna use the default lpuart1 port as console
		nice
		I think I have this https://mschoeffler.com/2017/10/05/tutorial-how-to-use-the-gy-521-module-mpu-6050-breakout-board-with-the-arduino-uno/ done on the nucleo board.
			Michael Schoeffler
		Michael Schoeffler
		Tutorial: How to use the GY - 521 module(MPU - 6050 breakout board) wi...
This tutorial teaches how to use the GY-521 Module(MPU - 6050 MEMS).The tutorial covers wiring as well as programming of the GY - 521 module.
  Tutorial: How to use the GY - 521 module(MPU - 6050 breakout board) wi...
Its a 'hello world' for the mpu6050 but it basically reads the data we need(except the thermostat values)
and tbh thats just two extra bytes soooo we can toss if if needed.
Gary — Yesterday at 1:15 PM
Nice!That’s great ??
codydeyarmin — Yesterday at 1:16 PM
digikey has some fucking awesome youtube and written tutorials for these nucleo boards
Gary — Yesterday at 1:16 PM
Also if I have to resort back to USART for comm, I can switch the USART to whatever
Would just potentially need to solder more, but we probably have to do that anyways
codydeyarmin — Yesterday at 1:16 PM
		if You're using usart I think its okay.
		the lpuart1 is uart and I think different.If not I'm sure I can find an alternate console out method.
		Gary — Yesterday at 1:18 PM
		Yeah it’ll all work out
That L4 board has so much ??
codydeyarmin — Yesterday at 1:19 PM
it really does
		codydeyarmin — Yesterday at 1:53 PM
		I am reading accelerometer data
		and the values make sense
		Also.Were gonna have a temperature value as its faster to just read it and ignore than to read around it.
If we want that somehow included
		codydeyarmin — Yesterday at 4:43 PM
		This dshot stuff uses dma
		eww
		OH, also I finally found a HAL api manual.
		looks like one is made for each family and has all the Hal functions and their descriptions.
		codydeyarmin — Yesterday at 5:09 PM
		We still planning on 1pm to meet tomorrow ?
		Gary — Yesterday at 5:10 PM
		yeah I'm still planning on it!
		codydeyarmin — Yesterday at 5:13 PM
		Awesome!  I should have most of my stuff started at the least.Looking at the dshot code now, it looks fairly straighforward to port to the L4.
		That covers accelerometer data in and esc / motor data out. With the pid control loop needing to be found and inserted in between the two.
		Gary — Yesterday at 5:14 PM
		do you want to come over to my apt.at courtyards?
codydeyarmin — Yesterday at 5:14 PM
		Ummm.Its up to yall. I can also host but your apartment is closer to campus for sure.
	   Gary — Yesterday at 5:16 PM
	   I've got a good amount of room to do some work (I'm in a 2x2 with a roommate).Courtyards is pretty spacious
		could lay everything out on my dining table
		codydeyarmin — Yesterday at 5:23 PM
		Ok!
		are you using ports PA0-3 ?
		Gary — Yesterday at 5:25 PM
		I'll list out the ports I'm using currently one sec
		codydeyarmin — Yesterday at 5:26 PM
		No worries.Im about to run to the store anyway.
Gary — Yesterday at 5:27 PM
USART:
PA9-- > Tx
		PA10-- > Rx
		
SPI1:
			(L4A6)
			PB3-- > SCLK
		PB4-- > MISO
		PB5-- > MOSI

		(ESP32)
		D18-- > SCLK
		D19-- > MISO
		D23-- > MOSI
		
LCD:
			PB2-- > D7
		PB1-- > D6
		PB15-- > D5
		PB14-- > D4
		PB13-- > E
		PB10-- > RS
		
TIM2
PA0 --> Channel 1
		PA1-- > Channel 2
		PA2-- > Channel 3
		PA3-- > Channel 4
		
ADC1:
			PC0-- >
			PC1-- >

			Gary
		 pinned
a message
 to this channel.See all
		pinned messages
.
 — Yesterday at 5:28 PM
codydeyarmin — Yesterday at 5:28 PM
Ok, thanks!
Gary — Yesterday at 5:28 PM
I won't use both SPI and UART, but tossing both in there
		codydeyarmin — Yesterday at 5:28 PM
		Add PA0 - 3 to that list as TIM2_CH1 - 4 output
			That will end up being the dshot outputs.
			Gary — Yesterday at 5:37 PM
			sweeeet
		not sure how late you are planning on staying @codydeyarmin but we can order Chinese or Pizza for dinner?
		my girlfriend will also be hanging around
		codydeyarmin — Yesterday at 5:38 PM
		That could work!
		Could you post a photo of your board here.I think im gonna solder pins onto mine and i wanna get both your set soldered too
		Triple XZ — Yesterday at 5:55 PM
		using PC0 and PC1 for ADC1 inputs
		i think I can reuse the same timers
		but if not we turn on TIM1 or TIM3
		codydeyarmin — Yesterday at 6:17 PM
		Ok.
		I was just hopping into ask if you could post the pins for the servo too ron!
		Triple XZ — Yesterday at 6:36 PM
		A1 and A3
		deliverygaming — Today at 11:03 AM
		Morning, what time and where we meeting today ?
		Gary — Today at 11:08 AM
		I’ll be back at my apartment in ~45 min.So anytime after that!
		8200 Boteler Lane, Apt 248, College Park, MD 20740
		Gary — Today at 11:38 AM
		Just soldered in some more male headers
		Gary — Today at 11:45 AM
		I’m back to my place, you guys can come over whenever
		Gary — Today at 11:56 AM
		Image
		My building is circled in blue, and the red lots are for visitor parking
		Triple XZ — Today at 12:06 PM
		dam kinda wishing i had my car now lol
		me and will might just call in
Gary — Today at 12:06 PM
		sweeeet
		Triple XZ — Today at 12:07 PM
		Akivaaa bring your vr lol
		deliverygaming — Today at 12:12 PM
		Will do.Gonna head out in a few
		codydeyarmin — Today at 12:14 PM
		Imma head over in about 15.
		Gary — Today at 12:14 PM
		@codydeyarmin do you have any solder stuff by chance?
actually we may be good
I accidentally soldered a GND and 5V together a little
		I was able to scrape them away tho
		codydeyarmin — Today at 12:23 PM
		Yee
		I was gonna bring my kit as i had soldering to do
				Gary — Today at 12:26 PM
				hell yeah okay
		codydeyarmin — Today at 12:32 PM
		Im bringing ron.
deliverygaming — Today at 12:39 PM
Just realized forgot the be
??
Triple XZ — Today at 12:41 PM
?
deliverygaming — Today at 12:41 PM
Turning around might be 10 late
Triple XZ — Today at 12:41 PM
oh VR
deliverygaming — Today at 12:41 PM
		Vr ^
		Triple XZ — Today at 12:41 PM
		hold up I got mine
		deliverygaming — Today at 12:42 PM
		Ok
		Gary — Today at 12:43 PM
		you got that gimble too @Triple XZ ??
		deliverygaming — Today at 12:44 PM
		I brought
		Triple XZ — Today at 12:44 PM
		yup
		codydeyarmin — Today at 12:56 PM
		Were gonna be like 1:10
		Gary — Today at 12:57 PM
		sounds good!
		deliverygaming — Today at 1:02 PM
		Just parked should be there in a few
		Gary — Today at 1:03 PM
		??
codydeyarmin — Today at 1:15 PM
Were outsude
TheTribalTurtle — Today at 1:54 PM
lemme know if you guys want to call me
Triple XZ — Today at 2:22 PM
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t VR[2];
/* USER CODE END 0 */

/**
Expand
temp_wow.txt
3 KB
codydeyarmin — Today at 2:57 PM
Attachment file type: archive
um1884-description-of-stm32l4l4-hal-and-lowlayer-drivers-stmicroelectronics.zip
7.12 MB
?
```csharp
using System;
using System.Collections; 
using System.Collections.Generic; 
using System.Net; 
using System.Net.Sockets; 
using System.Text; 
using System.Threading; 
using UnityEngine;  
using MsgPack.Serialization;
using System.Threading.Tasks;
using System.Windows;
using System.IO;

public class PhotoEntry
{
    public int[] Image { get; set; }
}

public class client : MonoBehaviour
{
	public Thread tcpListenerThread;
	public String host = "192.168.1.3";
    public Int32 port = 40810;

	internal Boolean socket_ready = false;
    internal String input_buffer = "";
	TcpClient tcp_socket;
    NetworkStream net_stream;

    StreamWriter socket_writer;
    StreamReader socket_reader;
		
	// Use this for initialization
	void Start () { 		
		// Start TcpServer background thread 		
		//ListenForIncommingRequests();
		tcpListenerThread = new Thread (new ThreadStart(ListenForIncommingRequests)); 		
		tcpListenerThread.IsBackground = true; 		
		tcpListenerThread.Start(); 	
	}  	
	
	// Update is called once per frame
	void Update () { 		
		// if (Input.GetKeyDown(KeyCode.Space)) {             
		// 	SendMessage();         
		// } 	
	}  	
	
	public void setupSocket() {
        try {
            tcp_socket = new TcpClient(host, port);
            net_stream = tcp_socket.GetStream();
            socket_writer = new StreamWriter(net_stream);
            socket_reader = new StreamReader(net_stream);
            socket_ready = true;
			Debug.Log("connected");
        } catch (Exception e) {
            Debug.Log("Socket error: " + e); // Something went wrong
        }
    }

	public void closeSocket() {
        if (!socket_ready)
			//Debug.Log("(closeSocket) socket not ready");
            return;

        socket_writer.Close();
        socket_reader.Close();
        tcp_socket.Close();
        socket_ready = false;
		Debug.Log("connection closed");
    }

	public void writeSocket(byte[] data) {
        for (int i=0; i < data.Length; i++) {
			net_stream.WriteByte(data[i]);
		}
		Debug.Log("Sent: " + string.Join(", ", data));
    }

	public byte[] readSocket() {
        byte[] bytes = new byte[100];
		int c;
		for (int i=0; i < 100; i++) {
			c = net_stream.ReadByte();
			bytes[i] = (byte) c;
			if (i > 4 && bytes[i] == 0x41 && bytes[i-1] == 0x38 && bytes[i-2] == 0x30 && bytes[i-3] == 0x34) {
				break;
			}
		}  
		
		Debug.Log("received: " + string.Join(", ", bytes));
        return bytes;
    }

	void ListenForIncommingRequests () { 
		try { 	
			Debug.Log("connecting...");
			setupSocket();        
			while (true) {
				// READ PART
				var da = readSocket();
				
				// SEND PART
				var data = new byte[] {149, 205, 1, 40, 205, 3, 14, 205, 1, 142, 205, 1, 144, 205, 1, 244};
				writeSocket(data);
			}

			// Debug.Log("starting serialization");
			// var targetObject = new PhotoEntry { Image = new int[] { 222, 111, 333, 444, 555}};

			// 1. Create serializer instance.
			// var serializer = MessagePackSerializer.Get<PhotoEntry>();

			// 2. Serialize object to the specified stream.
			// serializer.Pack(clientStream, targetObject);

			closeSocket();	
		} 		
		catch (SocketException socketException) { 			
			Debug.Log("SocketException " + socketException.ToString()); 		
		}     
	} 
    
}