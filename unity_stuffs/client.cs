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
		var memStream = new MemoryStream();
		
		// Debug.Log("starting serialization");
		var targetObject = new PhotoEntry { Image = new int[] { 222, 111, 333, 444, 555}};

		// 1. Create serializer instance.
		var serializer = MessagePackSerializer.Get<PhotoEntry>();

		// 2. Serialize object to the specified stream.
		serializer.Pack(memStream, targetObject);

		byte[] data2 = memStream.ToArray();

		for (int i=1; i < data2.Length; i++) {
			net_stream.WriteByte(data2[i]);
		}
		Debug.Log("Sent: " + string.Join(", ", data2));

        // for (int i=0; i < data.Length; i++) {
		// 	net_stream.WriteByte(data[i]);
		// }
		// Debug.Log("Sent: " + string.Join(", ", data));
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

			closeSocket();	
		} 		
		catch (SocketException socketException) { 			
			Debug.Log("SocketException " + socketException.ToString()); 		
		}     
	} 
    
}
