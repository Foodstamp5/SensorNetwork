<?php
  require_once 'utils.php';

  /*
   * This page will send a string to myserver through GET methods as follows:
   * ID: the unique ID of the ESP
   * STRING: the string to write
   */
  
  //Connect to my server
  $conn=connect();
  $table='text';

  //Pull string and value from $_GET
  $string="STRING";
  $val=$_GET[$string];
  
  //Create table if it doesn't exist
  $query="create table ".$table." (ID VARCHAR(32) not null, STRING VARCHAR(32) not null, ";
  $query=$query."LAST_TIME DATETIME default 0);";
  println("Create: ".$query);
  $conn->query($query);
  
  //Check to see if entry exists
  $query="select ".$string." from ".$table." where ID='".$_GET["ID"]."';";
  $result=$conn->query($query);
  println("Found $result->num_rows row(s)");
  if($result->num_rows>0){
    //At this point an entry exists
    $query="update ".$table." set ".$string."='".$val."' where ID='".$_GET["ID"]."';";
    println("Query: ".$query);
    if($conn->query($query)){
      println("Updated $string = $val successfully");
    }
    else{
      println("Unable to update $string to $val");
    }
  }else{
    //At this point we need to create an entry with the appropriate pin value
    $query="insert into ".$table." (ID,".$string.",LAST_TIME)values('".$_GET["ID"]."','$val',NOW());";
    println("Query: ".$query);
    if($conn->query($query)){
      println("Created index for ".$_GET["ID"]);
    }else{
      println("Could not create index for ".$_GET["ID"]);
    }
  }
  
  //Update timestamp
  $query="update ".$table." set LAST_TIME=NOW() where ID='".$_GET["ID"]."';";
  if($conn->query($query)){
    println("Timestamp updated successfully");
  }else{
    println("Timestamp not updated");
  }
  
  echo "\$_GET =<br>";
  foreach($_GET as $x=>$x_value){
    echo "Key=" . $x . ", Value=" . $x_value;
    echo "<br>";
  }
  
  //Print the result as $001011 where each 'bit' is either the echoed write value or fetched read value
  foreach($end_result as $result){
    echo $result;
  }