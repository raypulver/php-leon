--TEST--
should serialize an object in the right key order
--FILE--
<?php
$template = json_decode('{"woop":16,"xd":0,"ab":6,"aba":32,"zoop":0}', true);
$payload = json_decode('{"woop":"coop","xd":5,"ab":5.5,"aba":true,"zoop":7}', true);
$channel = new LEON\Channel($template);
$ser = $channel->encode($payload);
function bytesToString($bytes) {
  $ret = '';
  for ($i = 0; $i < count($bytes); ++$i) {
    $ret .= chr($bytes[$i]);
  }
  return $ret;
}
$thebytes = json_decode('[0,0,176,64,32,0,4,99,111,111,112,5,7]', true);
if (bytesToString($thebytes) === $ser) echo 'pass';
else echo 'fail';
?>
--EXPECT--
pass
