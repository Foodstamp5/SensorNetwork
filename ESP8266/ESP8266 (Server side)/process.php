<?php
  include 'utils.php';
    
  println("Procesing...");

  $conn=connect();
  $table='ESP8266';
  
  if($conn->connect_error){
      println("Connection failed: ".$conn->connect_error);
  }else{
      println("Connection successful");
  }

  //Create table if it doesn't exist
  $query="create table ".$table." (ID VARCHAR(32) not null,";
  $pins=array(0,1,2,3,4,5,12,13,14,15,16,'A');
  foreach($pins as $pin){
    if($pin==="A"){
      $query=$query."GPIO".$pin." smallint(10) unsigned default 0, ";
    }
    else{
      $query=$query."GPIO".$pin." bool default 0, ";
    }
  }
  $query=$query."LAST_TIME DATETIME default 0);";
  println("Create: ".$query);
  $conn->query($query);
  
  println("table: ".$table);
  
  if($_POST["delete"]=="no"){
    //Update mysql table with ID, GPIO0, GPIO2
//    $query="update ".$table." set GPIO0=".$_POST["GPIO0"]." set GPIO2=".$_POST["GPIO2"]." where ID='".$_POST["ID"]."';";
    $query="update ".$table." set ";//" set GPIO2=".$_POST["GPIO2"]." where ID='".$_POST["ID"]."';";
    foreach($pins as $pin){
      $query=$query."GPIO$pin=".$_POST["GPIO$pin"].", ";
    }
    $query=$query."LAST_TIME=now()";
    $query=$query." where ID='".$_POST["ID"]."';";
    println($query);
    if($conn->query($query)){
      println("Update successful");
    }else{
      println("Update failed");
    }
  }else{
    $query="delete from ".$table." where ID='".$_POST["ID"]."'";
    if($conn->query($query)){
      println("Entry deleted");
    }else{
      println("Entry didn't exist");
    }
  }
  
  //Close the connection
  $conn->close();
  
  echo '<script>window.top.location.reload();</script>'
?>