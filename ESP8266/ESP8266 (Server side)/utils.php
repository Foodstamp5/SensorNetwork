<?php
  function println($str){
      echo "<p>".$str."</p>";
  }

  global $table;
  function connect(){  
    $server='localhost';
    $username='iot';
    $password='nick-iot';
    $database='iot';
    return new mysqli($server,$username,$password,$database);
  }
  
  class ESP{
    function ESP($id_in,$time_in){
      $id=$id_in;
      $time=$time_in;
    }
    var $id;
    var $time;
    var $pins=array();
  }
?>