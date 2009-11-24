<?php
/**
 * ProcMonitor Class
 *
 * Simple process spawning and monitoring
 * Designed to be used in combination with AJAX for monitoring
 * the progress of server side executions
 * 
 * @author Jamie Wong <jamie.lf.wong@gmail.com>
 * @version 0.1 Updated Nov 13 2009
 * @link http://jamie-wong.com
 */

class ProcMonitor {
	/**
	 * Spawn the process, return the temp. file id and the process id
	 * @param string $cmd
	 * @return mixed array[0]:the unique id, array[1]:the process id
	 */
	function Start($cmd) {
		$id = md5(uniqid());

		$file = sprintf("/tmp/%s.txt",$id);
		$bytes_file = sprintf("/tmp/%s_bytesread.txt",$id);

		$file_handle = fopen($file,"w");
		fclose($file_handle);

		$bytes_file_handle = fopen($bytes_file,"w");
		fputs($bytes_file_handle,"0");
		fclose($bytes_file_handle);
	
		exec("$cmd > $file 2> $file & echo $!",$info);

		$pid = (int)$info[0];

		return array($id,$pid);
	}

	/**
	 * Read from the output of the process, return all new data since the last call
	 * @param string $id the id provided by ProcMonitor::Start
	 * @param int $pid the process id provided by ProcMonitor::Start
	 * @return mixed
	 * string array[0]: all the new data since the last call
	 * boolean array[1]: stating whether or not the process is still running
	 */
	function Read($id,$pid) {	
		$file = sprintf("/tmp/%s.txt",$id);
		$bytes_file = sprintf("/tmp/%s_bytesread.txt",$id);
		$file_handle = fopen($file,"r");
		$bytes_file_handle = fopen($bytes_file,"r");

		exec("ps -p $pid",$info);
		if (count($info) == 1) {
			// ps -p only displayed header: 
			//  command has terminated
			$running = false;
		} else {
			$running = true;
		}
	
		$bytesread = (int)fread($bytes_file_handl,filesize($bytes_file));
		fseek($file_handle,$bytes_read);
	

		$data = substr($file_out,$bytesread);

		if (!$running) {
			// Once the process has terminated
			//  delete the temporary files
			unlink($file);
			unlink($bytes_file);
		}

		return array($data,$running);
	}
}

?>
