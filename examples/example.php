<?
include("ProcMonitor.class.php");

list($fileId,$procId) = ProcMonitor::Start("locate home");

while (ProcMonitor::Running($procId) || ProcMonitor::OutputWaiting($fileId)) {
	print ProcMonitor::GetOutput($fileId);
}

ProcMonitor::Close($fileId,$procId);
?>
