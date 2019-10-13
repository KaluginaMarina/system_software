$groups=[Security.Principal.WindowsIdentity]::GetCurrent().Groups | foreach-object  {$_.Translate([Security.Principal.NTAccount]) }

$dir = @(Get-ChildItem -Path . | Where-Object {$_.PSIsContainer}) 


foreach ($d in $dir) {
	$getacl=(Get-Acl .).Access | ?{$_.AccessControlType -match "Allow"} | ?{($_.FileSystemRights -band [System.Security.AccessControl.FileSystemRights]::Delete) -match [System.Security.AccessControl.FileSystemRights]::Delete}
	:loop
	foreach ($group in $groups) { 
		foreach ($acl in $getacl) {
			if ( $group.equals($acl.IdentityReference) ) { 
				echo $($d.tostring())
				break loop
			} 
		}  
	} 
}