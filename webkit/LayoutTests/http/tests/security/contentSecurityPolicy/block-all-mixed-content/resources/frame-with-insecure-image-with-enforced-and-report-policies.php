<?php
    header("Content-Security-Policy-Report-Only: block-all-mixed-content");
    header("Content-Security-Policy: block-all-mixed-content");
?>
<!DOCTYPE html>
<html>
<head>
<script src="dump-securitypolicyviolation-and-notify-done.js"></script>
</head>
<body>
<img src="http://127.0.0.1:8000/security/resources/compass.jpg">
</body>
</html>