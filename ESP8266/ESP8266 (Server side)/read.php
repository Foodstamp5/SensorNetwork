<?php
  require_once 'utils.php';
  
  function bool($int){
    if($int){
      return 'HIGH';
    }
    else{
      return 'LOW';
    }
  }

  echo '<head><META HTTP-EQUIV="refresh" CONTENT="1"></head>';
  println("The values of the ESP8266 are shown below");

  $conn=connect();
  $table='ESP8266';
  $pins=array(0,1,2,3,4,5,12,13,14,15,16,'A');
    
//    if($conn->connect_error){
//      println("Connection failed: ".$conn->connect_error);
//    }else{
//      println("Connection successful");
//    }
  
    $query="select * from ".$table.";";
//    println($query);
    $result=$conn->query($query);
//    println("WHY?");
//    println($result->num_rows);
//    println("HERE");
    if($result->num_rows>0){
      echo '<table style="text-align:center;border:1px solid black">';
      echo  '<tr>';
      echo    '<td> ID </td>';
      foreach($pins as $pin){
        echo "<td> GPIO$pin </td>";
      }
      echo    '<td>Last Seen</td>';
      echo  '</tr>';
      while($row=$result->fetch_assoc()){
        $time=time()-strtotime($row["LAST_TIME"]);
        $time_str=($time%60)." sec ago";
        if($time>60) $time_str=(($time/60)%60)." min ".$time_str;
        if($time>60*60) $time_str=(($time/(60*60))%24)." hr ".$time_str;
        if($time>60*60*24) $time_str=(floor($time/(60*60*24)))." dy ".$time_str;
        echo "<tr>";
        echo "<td>".$row["ID"]."</td>";
        foreach($pins as $pin){
          echo "<td>".$row["GPIO$pin"]."</td>";
        }
        echo "<td>".$time_str."</td>";
      }
    }
    else{
      println("No records");
    }
    echo '</table>';
//  }
?>