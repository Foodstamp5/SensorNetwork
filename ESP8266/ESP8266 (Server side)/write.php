<?php
  require_once 'utils.php';
  
  //Connect to my server
  $conn=connect();
  $table='ESP8266';
  $pins=[0,1,2,3,4,5,12,13,14,15,16,'A'];
  
  //Read in GPIO values to set form values
//  $query="select ID,GPIO0,GPIO2,LAST_TIME from ".$table.";";
  $query="select * from $table;";
  $result=$conn->query($query);
//  println($query);
  $table=array();
  if($result->num_rows>0){
//    $result->fetch_assoc(); //Discard header
    while($row=$result->fetch_assoc()){
      //Load table into $table
      foreach($row as $key=>$val){
        $table[$key][]=$val;
      }
    }
  }

  println("Update the values of the ESP8266 below");
  echo '<form action="process.php" method="post">';
//    echo  'ID: <input type="text" name="ID"> <br>';
  echo  'ID: <select name="ID">';
  foreach ($table["ID"] as $value) {
    echo '<option value="'.$value.'">'.$value.'</option>';  
  }
  echo '</select><br>';
  
  foreach($pins as $p){
    if($p==='A'){//Analog
      echo  'GPIO'.$p.': <input name="GPIO'.$p.'" id="gpio'.$p.' type="number" min="0" max="1023" value=0 maxlength=4 size=4>';     
    }else{//Digial 
      echo  'GPIO'.$p.': <select name="GPIO'.$p.'" id="gpio'.$p.'">';
      echo    '<option value="0">LOW</option>';
      echo    '<option value="1">HIGH</option>';
      echo  '</select>';
    }
    echo '<br>';
  }

  echo '<input type="submit" value="Update"><br>';
  echo '<input type="radio" name="delete" value="no" checked>Update Value<br>';
  echo '<input type="radio" name="delete" value="yes">Delete Enrty<br>';
  echo '</form>';

  //TODO update the form to hold the existing info every time the ID is changed
?>