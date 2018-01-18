import sys
import os
import signal

# Start off fresh by making sure that our working directory is the same as the
# directory that this script is in.
os.chdir(os.path.dirname(os.path.realpath(__file__)))

sys.path.insert(0, '../util')
import process_manager

ground_processes = None

def signal_received(signal, frame):
    ground_processes.killall()
    sys.exit(0)

if __name__ == "__main__":
    ground_processes = process_manager.ProcessManager()

    ground_processes.spawn_process("python server/run_backend.py")
    # ground_processes.spawn_process("npm start --prefix ./client/")
    ## Sockets aren't working -- see serve client

    ## Run this command to build final version
    ground_processes.spawn_process("python client/serve_client.py")

    signal.signal(signal.SIGINT, signal_received)

    signal.pause()
