using System;
using System.Collections.Generic;
using System.IO.Ports;
using System.Linq;
using System.Net.Http;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace DataReader
{
    class Program
    {
        private static readonly HttpClient client = new HttpClient();
        static void Main(string[] args)
        {
            //setting up serial port
            SerialPort port = new SerialPort();
            port.BaudRate = 9600;
            port.PortName = "COM6"; 
            
            while(true)
            {
                try //trying to establish connection
                {
                    port.Open();
                    Console.Clear();
                    Console.WriteLine("Connected...");
                    while (true)
                    {
                        string data = port.ReadLine();
                        if (data != null)
                        {
                            //reading radio message
                            string[] tokens = data.Split('/').ToArray();

                            string gateway_id = tokens[0];
                            string station_id = tokens[1];
                            string magnitude = tokens[2];
                            string orientation = tokens[3];
                            string latitude = tokens[4];
                            string longitude = tokens[5];

                            var values = new Dictionary<string, string> //radio to http
                            {
                                { "longitude", longitude },
                                { "latitude", latitude },
                                { "magnitude", magnitude },
                                { "orientation", orientation }
                            };

                            //sending web post
                            var content = new FormUrlEncodedContent(values);

                            var response = client.PostAsync("https://localhost:44302/Earthquakes/Receive", content);

                            var responseString1 = response.Result;

                            Console.WriteLine(data);
                        }
                    }
                }
                catch //checking connection
                {
                    port.Close();
                    Console.Clear();
                    Console.WriteLine("Unable to connect...");
                    Thread.Sleep(5000);
                }
            }
        }
    }
}
