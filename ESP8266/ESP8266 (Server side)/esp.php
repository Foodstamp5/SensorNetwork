<?php
  require_once 'utils.php';

  /*
   * This page will process all esp data through GET methods as follows:
   * ID: the unique ID of the ESP
   * GPIOx=0: clear the GPIO pin desingated by x, return 0
   * GPIOx=1: set the GPIO pin desingated by x, return 1
   * GPIOx=R: red the GPIO pin desingated by x and return it
   */
  
  //Connect to my server
  $conn=connect();
  $table='ESP8266';
  $mark='$';  //Marks the beginning of the return data
  
  $end_result=array($mark);

  //Read the GPIO pin for the ESP if passed a 'R'
  //Sets the GPIO value if passed a '1'
  //Clears the GPIO value if passed a '0'
  foreach($_GET as $pin=>$val){
    //If we are reading values to the ESP
    if($val=='R'||$val=='r'){
      $query="select ".$pin." from ".$table." where ID='".$_GET["ID"]."';";
      $result=$conn->query($query);
    //  println($query);
      println("Found $result->num_rows row(s)");
      if($result->num_rows>0){
        //Read the value out to the ESP
        $row=$result->fetch_assoc();
        $end_result[]=$row[$pin];
      }
      else{
        //Read a defult value to the ESP
        $end_result[]="0";

        //Create an index for the ESP
        $query="insert into ".$table." (ID,".$pin.",LAST_TIME)values('".$_GET["ID"]."',0,NOW());";
        println("Query: ".$query);
        if($conn->query($query)){
          println("Created index for ".$_GET["ID"]);
        }else{
          println("Could not create index for ".$_GET["ID"]);
        }
      }
    }//If we are writing a '0' or '1'
    else if($val=='0' || $val=='1'){
      //Check to see if entry exists
      $query="select ".$pin." from ".$table." where ID='".$_GET["ID"]."';";
      $result=$conn->query($query);
      println("Found $result->num_rows row(s)");
      if($result->num_rows>0){
        //At this point an entry exists
        $query="update ".$table." set ".$pin."=".$val." where ID='".$_GET["ID"]."';";
        println("Query: ".$query);
        if($conn->query($query)){
          println("Updated $pin = $val successfully");
        }
        else{
          println("Unable to update $pin to $val");
        }
      }else{
        //At this point we need to create an entry with the appropriate pin value
        $query="insert into ".$table." (ID,".$pin.",LAST_TIME)values('".$_GET["ID"]."',$val,NOW());";
        println("Query: ".$query);
        if($conn->query($query)){
          println("Created index for ".$_GET["ID"]);
        }else{
          println("Could not create index for ".$_GET["ID"]);
        }
      }
      //Echo the write value back
      $end_result[]=$val;
    }else{//Should pickup ID and LAST_TIME cases
      println("Not doing anything for $pin=>$val");
    }
  }

  //Update timestamp
  $query="update ".$table." set LAST_TIME=NOW() where ID='".$_GET["ID"]."';";
  if($conn->query($query)){
    println("Timestamp updated successfully");
  }else{
    println("Timestamp not updated");
  }
  
  echo "_GET =<br>";
  foreach($_GET as $x=>$x_value){
    echo "Key=" . $x . ", Value=" . $x_value;
    echo "<br>";
  }
  
  //Print the result as $001011 where each 'bit' is either the echoed write value or fetched read value
  foreach($end_result as $result){
    echo $result;
  }
?>