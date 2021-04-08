<?php
 $content = file_get_contents('php://input');
//  $foldername = $_GET["foldername"];
 $filename = $_GET["filename"];
 $fp = fopen($filename,'w+');
 fwrite($fp, $content, strlen($content));
 fclose($fp);
?>