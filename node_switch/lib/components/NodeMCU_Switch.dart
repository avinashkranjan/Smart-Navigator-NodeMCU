import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';
import 'package:web_socket_channel/io.dart';
import 'package:device_apps/device_apps.dart';
import 'package:url_launcher/url_launcher.dart';
import 'package:android_intent_plus/android_intent.dart';
import 'package:external_app_launcher/external_app_launcher.dart';

class NodeMCU_Switch extends StatefulWidget {
  @override
  State<StatefulWidget> createState() {
    return _NodeMCU_Switch();
  }
}

class _NodeMCU_Switch extends State<NodeMCU_Switch> {
  late bool ledstatus; //boolean value to track LED status, if its ON or OFF
  late IOWebSocketChannel channel;
  late bool connected; //boolean value to track if WebSocket is connected

  @override
  void initState() {
    ledstatus = false; //initially leadstatus is off so its FALSE
    connected = false; //initially connection status is "NO" so its FALSE

    Future.delayed(Duration.zero, () async {
      channelconnect(); //connect to WebSocket wth NodeMCU
    });

    super.initState();
  }

  channelconnect() {
    //function to connect
    try {
      channel =
          IOWebSocketChannel.connect("ws://192.168.0.1:81"); //channel IP : Port
      channel.stream.listen(
        (message) {
          print(message);
          setState(() {
            if (message == "connected") {
              connected = true; //message is "connected" from NodeMCU
            } else if (message == "poweron") {
              ledstatus = true;
            } else if (message == "poweroff") {
              ledstatus = false;
            }
          });
        },
        onDone: () {
          //if WebSocket is disconnected
          print("Web socket is closed");
          setState(() {
            connected = false;
          });
        },
        onError: (error) {
          print(error.toString());
        },
      );
    } catch (err) {
      print(err.toString());
      print("error on connecting to websocket.");
    }
  }

  Future<void> sendcmd(String cmd) async {
    if (connected == true) {
      if (ledstatus == false && cmd != "poweron" && cmd != "poweroff") {
        print("Send the valid command");
      } else {
        channel.sink.add(cmd); //sending Command to NodeMCU
      }
    } else {
      channelconnect();
      print("Websocket is not connected.");
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
          title: Text("LED - ON/OFF NodeMCU"),
          backgroundColor: Colors.redAccent),
      body: Container(
          alignment: Alignment.topCenter, //inner widget alignment to center
          padding: EdgeInsets.all(20),
          child: Column(
            children: [
              Container(
                  child: connected
                      ? const Text("WEBSOCKET: CONNECTED")
                      : const Text("WEBSOCKET: DISCONNECTED")),
              Container(
                  child: ledstatus
                      ? const Text("LED IS: ON")
                      : const Text("LED IS: OFF")),
              Container(
                  margin: EdgeInsets.only(top: 30),
                  child: FlatButton(
                      //button to start scanning
                      color: Colors.redAccent,
                      colorBrightness: Brightness.dark,
                      onPressed: () async {
                        // Returns a list of only those apps that have launch intent
                        List<Application> apps =
                            await DeviceApps.getInstalledApplications(
                                onlyAppsWithLaunchIntent: true,
                                includeSystemApps: true);
                        print(apps);
                        //on button press
                        if (ledstatus) {
                          //if ledstatus is true, then turn off the led
                          //if led is on, turn off
                          sendcmd("poweroff");
                          ledstatus = false;
                        } else {
                          //if ledstatus is false, then turn on the led
                          //if led is off, turn on
                          sendcmd("poweron");
                          ledstatus = true;
                        }
                        setState(() {
                          // await LaunchApp.openApp(
                          //   androidPackageName: 'com.team4.helping_hands',
                          //   // iosUrlScheme: 'pulsesecure://',
                          //   // appStoreLink:
                          //   //     'itms-apps://itunes.apple.com/us/app/pulse-secure/id945832041',
                          //   // openStore: false
                          // );
                          // _openHelperApp({"helping_hands": "https://www.google.com"});
                          // DeviceApps.openApp('com.team4.helping_hands');
                        });
                      },
                      child: ledstatus
                          ? const Text("TURN LED OFF")
                          : const Text("TURN LED ON")))
            ],
          )),
    );
  }
}
