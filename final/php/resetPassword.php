<?php
	session_start();
	$_SESSION["RegState"] = 6;
	header("location:../index.php");
	exit();
?>