import 'package:flutter/material.dart';
import 'components/NodeMCU_Switch.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'Node Switch',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: NodeMCU_Switch(),
    );
  }
}
