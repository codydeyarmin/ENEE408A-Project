using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using UnityEngine;
//using MsgPack.Serialization;
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

	public GameObject headset;
	private Vector2 leftinput;
	private Vector2 rightinput;
	public int forward;
	public int right;
	public int up;
	public int rotation;
	public int panup;
	public int panright;
	private byte[] forward_bytes;
	private byte[] right_bytes;
	private byte[] up_bytes;
	private byte[] rotation_bytes;
	private byte[] panup_bytes;
	private byte[] panright_bytes;

	internal Boolean socket_ready = false;
	internal String input_buffer = "";
	TcpClient tcp_socket;
	NetworkStream net_stream;

	StreamWriter socket_writer;
	StreamReader socket_reader;

	// Use this for initialization
	void Start()
	{
		// Start TcpServer background thread 		
		//ListenForIncommingRequests();
		tcpListenerThread = new Thread(new ThreadStart(ListenForIncommingRequests));
		tcpListenerThread.IsBackground = true;
		tcpListenerThread.Start();
		forward = 0;
		right = 0;
		up = 0;
		rotation = 0;
		panup = 0;
		panright = 0;
	}

	// Update is called once per frame
	void Update()
	{
		// if (Input.GetKeyDown(KeyCode.Space)) {             
		// 	SendMessage();         
		// } 	
		leftinput = OVRInput.Get(OVRInput.Axis2D.PrimaryThumbstick);
		rightinput = OVRInput.Get(OVRInput.Axis2D.SecondaryThumbstick);

		forward = (int)(leftinput.y * 1000) + 1000;
		right = (int)(leftinput.x * 1000) + 1000;
		up = (int)(rightinput.y * 1000) + 1000;
		rotation = (int)(rightinput.x * 1000) + 1000;
		panup = (int)headset.transform.localEulerAngles.x;
		panright = (int)headset.transform.localEulerAngles.y;
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
			while (true)
			{
				// READ PART
				var da = readSocket();
				panup_bytes = BitConverter.GetBytes(panup);
				panright_bytes = BitConverter.GetBytes(panright);
				forward_bytes = BitConverter.GetBytes(forward);
				right_bytes = BitConverter.GetBytes(right);
				up_bytes = BitConverter.GetBytes(up);
				rotation_bytes = BitConverter.GetBytes(rotation);
				if (BitConverter.IsLittleEndian)
                {
					Array.Reverse(panup_bytes);
					Array.Reverse(panright_bytes);
					Array.Reverse(forward_bytes);
					Array.Reverse(right_bytes);
					Array.Reverse(up_bytes);
					Array.Reverse(rotation_bytes);
				}
					

				// SEND PART
				// this currently does not use msgpack, but uses msgpack format
				var data = new byte[] { 150, 
					205, panright_bytes[0], panright_bytes[1], // pan right left
					205, panup_bytes[0], panup_bytes[1], // pan up down
					205, forward_bytes[0], forward_bytes[1], // forward backward
					205, right_bytes[0], right_bytes[1], // left right
					205, up_bytes[0], up_bytes[1], // up down
					205, rotation_bytes[0], rotation_bytes[1], // yaw
					52, 48, 56, 65, // 0x4, 0x0, 0x8, 0xA
					1, 1, 1, 1, 1, 1, 1 // pad with 0x91
                    }; // TODO <---------------------------------------------------------------------------------------------
				writeSocket(data);
			}

			// Debug.Log("starting serialization");
			// var targetObject = new PhotoEntry { Image = new int[] { 222, 111, 333, 444, 555}};

			// 1. Create serializer instance.
			// var serializer = MessagePackSerializer.Get<PhotoEntry>();

			// 2. Serialize object to the specified stream.
			// serializer.Pack(clientStream, targetObject);

			//closeSocket();	
		}
		catch (SocketException socketException)
		{
			Debug.Log("SocketException " + socketException.ToString());
		}
	}

}