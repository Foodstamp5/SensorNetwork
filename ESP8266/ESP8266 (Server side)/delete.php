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
  $create_table=$conn->query("delete from ".$table." where ID=".$id.";");
  $conn->query($create_table);
  
  println("table: ".$table);
  
  //Delete any old data
  $query="delete from ".$table." where ID='".$_POST["ID"]."'";
  $conn->query($query);
  
  //Update mysql table with ID, GPIO0, GPIO2
  $query="insert into ".$table." values('".$_POST["ID"]."',".$_POST["GPIO0"].",".$_POST["GPIO2"].");";
  if($conn->query($query)){
    println("Update successful");
  }else{
    println("Update failed");
  }
  

  
  //Close the connection
  $conn->close();
  
  echo '<script>window.top.location.reload();</script>'
?>