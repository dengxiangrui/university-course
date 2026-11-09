Add-Type @"
using System;
using System.Text;
using System.Runtime.InteropServices;
public class Win32 {
    public delegate bool EnumProc(IntPtr h, IntPtr p);
    [DllImport("user32.dll")] public static extern bool EnumWindows(EnumProc cb, IntPtr p);
    [DllImport("user32.dll")] public static extern int GetWindowText(IntPtr h, StringBuilder s, int n);
    [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr h);
    [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr h);
    [DllImport("user32.dll")] public static extern bool ShowWindow(IntPtr h, int cmd);
    [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr h, out RECT r);
    [StructLayout(LayoutKind.Sequential)] public struct RECT { public int L, T, R, B; }
    public static IntPtr Find(string key) {
        IntPtr found = IntPtr.Zero;
        EnumWindows((h, p) => {
            var sb = new StringBuilder(512);
            GetWindowText(h, sb, 512);
            if (IsWindowVisible(h) && sb.ToString().Contains(key)) { found = h; return false; }
            return true;
        }, IntPtr.Zero);
        return found;
    }
}
"@
Add-Type -AssemblyName System.Drawing

$name = $args[0]
$h = [Win32]::Find("CG Lab1")
if ($h -eq [IntPtr]::Zero) { Write-Output "window not found"; exit 1 }
[Win32]::ShowWindow($h, 9) | Out-Null
[Win32]::SetForegroundWindow($h) | Out-Null
Start-Sleep -Milliseconds 1000
$r = New-Object Win32+RECT
[Win32]::GetWindowRect($h, [ref]$r) | Out-Null
$w = $r.R - $r.L; $ht = $r.B - $r.T
$bmp = New-Object System.Drawing.Bitmap($w, $ht)
$g = [System.Drawing.Graphics]::FromImage($bmp)
$g.CopyFromScreen($r.L, $r.T, 0, 0, (New-Object System.Drawing.Size($w, $ht)))
$out = Join-Path $PSScriptRoot $name
$bmp.Save($out, [System.Drawing.Imaging.ImageFormat]::Png)
$g.Dispose(); $bmp.Dispose()
Write-Output "saved $out ($w x $ht)"
