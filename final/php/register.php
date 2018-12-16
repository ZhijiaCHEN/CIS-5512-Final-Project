<?php
	session_start();
	$_SESSION["RegState"] = 1;
	header("location:../index.php");
	exit();
?>