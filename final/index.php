<?php
	session_start();
	if (!isset($_SESSION["RegState"])) {
		$_SESSION["RegState"] = 0;
	}
?>
<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta name="description" content="">
    <meta name="author" content="">
    <link rel="icon" href="images/favicon.ico">

    <title>CIS5512 Final Project</title>

    <!-- Bootstrap core CSS -->
    <link href="css/bootstrap.min.css" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="css/signin.css" rel="stylesheet">
  </head>

  <body class="text-center">
	<div class="row">
<?php
	if ($_SESSION["RegState"] == 0) {
?>
		<form  class="col form-signin">
		  <img class="mb-4" src="https://getbootstrap.com/docs/4.0/assets/brand/bootstrap-solid.svg" alt="" width="72" height="72">
		  <h1 class="h3 mb-3 font-weight-normal">CIS5512 Final Project (**Enter group id**)</h1>
		  <label for="inputEmail" class="sr-only">Email address</label>
		  <input type="email" id="inputEmail" class="form-control" placeholder="Email address" required autofocus>
		  <label for="inputPassword" class="sr-only">Password</label>
		  <input type="password" id="inputPassword" class="form-control" placeholder="Password" required>
		  <div class="checkbox mb-3">
			<label>
			  <input type="checkbox" value="remember-me"> Remember me
			</label>
		  </div>
		  <a href="dashboard.php"><button class="btn btn-lg btn-primary btn-block" type="button">Sign in</button></a>
		  <div id="messageLogin" class="alert alert-info mt-2" role="alert"></div>
		  <a href="php/register.php">Register</a> | <a href="php/resetPassword.php">Forget?</a>
		</form>
<?php
	} 
	if ($_SESSION["RegState"] == 1) {  // When "register" is clicked
?>
		<form action="php/register.php" method="get" class="col form-signin">
		  <img class="mb-4" src="https://getbootstrap.com/docs/4.0/assets/brand/bootstrap-solid.svg" alt="" width="72" height="72">
		  <h1 class="h3 mb-3 font-weight-normal">CIS5512 Final Projecr Registration</h1>
		  <label for="inputFirstName" class="sr-only">First Name</label>
		  <input type="text" id="inputFirstName" class="form-control" placeholder="First Name" required autofocus>
		  <label for="inputLastName" class="sr-only">Last Name</label>
		  <input type="text" id="inputLastName" class="form-control" placeholder="Last Name" required>
		  <label for="inputEmail" class="sr-only">Email address</label>
		  <input type="email" id="inputEmail" class="form-control" placeholder="Email address" required autofocus>
		  <button type="submit" class="btn btn-lg btn-primary btn-block" type="submit">Register</button>
		  <div id="messageRegister" class="alert alert-info mt-2" role="alert"></div>
		  <a href="php/clearAll.php"><button type="button" class="mt-2">Return</button></a>
		</form>
<?php
	}
	if ($_SESSION["RegState"] == 6) { // when "forget" is clicked
?>
		<form action="php/resetPassword.php" method="get" class="col form-signin">
		  <img class="mb-4" src="https://getbootstrap.com/docs/4.0/assets/brand/bootstrap-solid.svg" alt="" width="72" height="72">
		  <h1 class="h3 mb-3 font-weight-normal">CIS5512 Final Project Reset Password</h1>
		  <label for="inputEmail" class="sr-only">Registered Email address</label>
		  <input type="email" id="inputEmail" class="form-control" placeholder="Registered email address" required autofocus>
		  <button type="submit" class="btn btn-lg btn-primary btn-block" type="submit">Authenticate</button>
		  <div id="messageResetPassword" class="alert alert-info mt-2" role="alert"></div>
		  <a href="php/clearAll.php"><button type="button" class="mt-2">Return</button></a>
		</form>
<?php
	}
	if ($_SESSION["RegState"] == 2) { // after email is authenticated
?>
		<form action="php/setPassword.php" method="get" class="col form-signin">
		  <img class="mb-4" src="https://getbootstrap.com/docs/4.0/assets/brand/bootstrap-solid.svg" alt="" width="72" height="72">
		  <h1 class="h3 mb-3 font-weight-normal">CIS5512 Final Project Set Password</h1>
		  <input type="hidden" id="inputEmail" class="form-control" value="<?php $_SESSION["Email"]; ?>">
		  <label for="inputPassword" class="sr-only">Enter a password</label>
		  <input type="password" id="inputPassword" class="form-control" placeholder="Password" required>
		  <button type="submit" class="btn btn-lg btn-primary btn-block" type="submit">Set Password</button>
		  <div id="messageSetPassword" class="alert alert-info mt-2" role="alert"></div>
		  <a href="php/clearAll.php"><button type="button" class="mt-2">Login</button></a>
		</form>
<?php
	}
?>
	</div>
	
    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="js/jquery-3.2.1.slim.min.js"></script>
    <script>window.jQuery || document.write('<script src="js/jquery-slim.min.js"><\/script>')</script>
    <script src="js/popper.min.js"></script>
    <script src="js/bootstrap.min.js"></script>
</body>
</html>