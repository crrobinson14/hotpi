<?php

/**
 * @file Processing daemon for poll vote counting
 *
 * This daemon was designed to be executed directly from the command line.
 */

$database = array(
  'host' => '127.0.0.1',
  'username' => 'hotpi',
  'password' => 'hotpi',
  'database' => 'hotpi',
);

set_time_limit(0);

$dbconn = new mysqli($database['host'], $database['username'], $database['password'], $database['database']);
if ($dbconn->connect_errno) die('Could not connect to database.');

/*  $stmt = $dbconn->prepare("SELECT value FROM variable WHERE name='wwe_widgets_poll_servers'");
  $stmt->execute();
  $stmt->bind_result($value);
  $stmt->fetch();
  $stmt->close();

  $servers = explode(',', unserialize($value));
  foreach ($servers as $key => $value) {
    $servers[$key] = trim($value);

    if (strlen($servers[$key]) < 1) {
      unset($servers[$key]);
    }
  }*/

/**
 * Main loop
 */
while (TRUE) {
  usleep(250000);

  foreach($servers as $server) {
    // Sample response:
    // {"results":[{"poll":26123456,"option":26654321,"count":8},{"poll":26000043,"option":26008816,"count":1}]}
    $result = json_decode(file_get_contents('http://' . $server . '/out/process.php?key=FVqNtdTAT5'));
    if ($result && is_object($result) && isset($result->results) && is_array($result->results)) {
      foreach ($result->results as $entry) {
        // Make sure the poll ID is valid and the poll is published.
        $stmt = $dbconn->prepare("SELECT count(n.nid) AS cnt FROM node n WHERE (n.type='poll' OR n.type='trivia' OR n.type='match') AND n.status=1 AND n.nid=?");
        $stmt->bind_param('i', $entry->poll);
        $stmt->execute();
        $stmt->bind_result($count);
        $stmt->fetch();
        $stmt->close();
        unset($stmt);
        if ($count < 1) continue;

        // TODO: Check for expiration date here.ƒ

        // Make sure the option is a valid entry for this poll
        $stmt = $dbconn->prepare("SELECT count(c.entity_id) AS cnt FROM field_data_field_poll_options_collection c WHERE c.field_poll_options_collection_value=? AND c.entity_id=?");
        $stmt->bind_param('ii', $entry->option, $entry->poll);
        $stmt->execute();
        $stmt->bind_result($count);
        $stmt->fetch();
        $stmt->close();
        unset($stmt);
        if ($count < 1) continue;

        // Update the entry
        $stmt = $dbconn->prepare("INSERT INTO wwe_voting_results (nid, field_id, value) VALUES (?, ?, ?) ON DUPLICATE KEY UPDATE value=value+?");
        $stmt->bind_param('iiii', $entry->poll, $entry->option, $entry->count, $entry->count);
        $stmt->execute();
        $stmt->close();
        unset($stmt);
      }
    }

    unset($result);
  }

  sleep(5);
}