import tkinter as tk
from tkinter import filedialog, messagebox
import subprocess
import threading
import os

def browse_output():
    path = filedialog.askdirectory()
    if path:
        output_var.set(path)

def start_download():
    url = url_var.get().strip()
    output = output_var.get().strip()
    threads = threads_var.get()

    if not url:
        messagebox.showerror("Error", "Please enter a URL!")
        return
    if not output:
        messagebox.showerror("Error", "Please select an output folder!")
        return

    btn_download.config(state="disabled")
    status_var.set("Downloading...")
    log_text.delete(1.0, tk.END)

    def run():
        exe = os.path.join(os.path.dirname(os.path.abspath(__file__)), "build", "downloader.exe")
        out_path = output
        cmd = [exe, url, out_path, str(threads)]
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        for line in process.stdout:
            log_text.insert(tk.END, line)
            log_text.see(tk.END)
        process.wait()
        status_var.set("Done!")
        btn_download.config(state="normal")

    threading.Thread(target=run, daemon=True).start()

root = tk.Tk()
root.title("Video Downloader")
root.geometry("600x500")
root.resizable(False, False)

tk.Label(root, text="URL:").pack(anchor="w", padx=10, pady=(10,0))
url_var = tk.StringVar()
tk.Entry(root, textvariable=url_var, width=80).pack(padx=10)

tk.Label(root, text="Output Folder:").pack(anchor="w", padx=10, pady=(10,0))
output_frame = tk.Frame(root)
output_frame.pack(fill="x", padx=10)
output_var = tk.StringVar(value=os.path.expanduser("~/Downloads"))
tk.Entry(output_frame, textvariable=output_var, width=65).pack(side="left")
tk.Button(output_frame, text="Browse", command=browse_output).pack(side="left", padx=5)

tk.Label(root, text="Quality (YouTube only):").pack(anchor="w", padx=10, pady=(10,0))
threads_var = tk.IntVar(value=4)
quality_frame = tk.Frame(root)
quality_frame.pack(anchor="w", padx=10)
for label, val in [("480p", 1), ("720p", 2), ("1080p", 3), ("Best", 4)]:
    tk.Radiobutton(quality_frame, text=label, variable=threads_var, value=val).pack(side="left")

btn_download = tk.Button(root, text="Download", command=start_download, bg="green", fg="white", width=20)
btn_download.pack(pady=10)

status_var = tk.StringVar(value="Ready")
tk.Label(root, textvariable=status_var, fg="blue").pack()

tk.Label(root, text="Log:").pack(anchor="w", padx=10)
log_text = tk.Text(root, height=15, width=72)
log_text.pack(padx=10, pady=5)

root.mainloop()
