--TEST--
Bug #34810 (mysqli::init() and others use wrong $this pointer without checks)
--SKIPIF--
<?php
require_once('skipif_azure_basic.inc');
?>
--FILE--
<?php

class DbConnection {
	public function connect() {
		require_once("connect.inc");

		$link = my_mysqli_connect($host, $user, $passwd, $db, $port, $socket);

		$link = mysqli_init();

		$mysql = new my_mysqli($host, $user, $passwd, $db, $port, $socket);
		$mysql->query("DROP TABLE IF EXISTS test_warnings");
		$mysql->query("CREATE TABLE test_warnings (a int not null)");
		$mysql->query("SET sql_mode=''");
		$mysql->query("INSERT INTO test_warnings VALUES (1),(2),(NULL)");

		$warning = $mysql->get_warnings();
		if (!$warning)
			printf("[001] No warning!\n");

		if ($warning->errno == 1048 || $warning->errno == 1253) {
			/* 1048 - Column 'a' cannot be null, 1263 - Data truncated; NULL supplied to NOT NULL column 'a' at row */
			if ("HY000" != $warning->sqlstate)
				printf("[003] Wrong sql state code: %s\n", $warning->sqlstate);

			if ("" == $warning->message)
				printf("[004] Message string must not be empty\n");


		} else {
			printf("[002] Empty error message!\n");
			var_dump($warning);
		}
	}
}

$db = new DbConnection();
$db->connect();

echo "Done\n";
?>
--CLEAN--
<?php
require_once("connect.inc");
if (!$link = my_mysqli_connect($host, $user, $passwd, $db, $port, $socket))
   printf("[c001] [%d] %s\n", mysqli_connect_errno(), mysqli_connect_error());

if (!mysqli_query($link, "DROP TABLE IF EXISTS test_warnings"))
	printf("[c002] Cannot drop table, [%d] %s\n", mysqli_errno($link), mysqli_error($link));

mysqli_close($link);
?>
--EXPECTF--
Done
